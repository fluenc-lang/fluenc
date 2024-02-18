#include <numeric>

#include <llvm/IR/Constants.h>

#include "AggregateIteratorValue.h"
#include "IRBuilderEx.h"
#include "ast.hpp"
#include "Emitter.h"
#include "utility.hpp"
#include "ValueHelper.h"

#include "iterators/index_iterator.hpp"

#include "types/IteratorType.h"
#include "types/Int64Type.h"
#include "types/ExpandedType.h"

#include "values/TupleValue.h"
#include "values/ExpandableValue.h"
#include "values/WithoutValue.h"
#include "values/PlaceholderValue.h"
#include "values/ScalarValue.h"
#include "values/ReferenceValue.h"
#include "values/ExpandedValue.h"

#include "fragments/ArrayContinuationFragment.h"
#include "fragments/StaticNode.h"
#include "fragments/pre_evaluation.hpp"
#include "fragments/post_evaluation.hpp"

AggregateIteratorValue::AggregateIteratorValue(const reference_value *index, const fluenc::expression_t &node, const Type *arrayType, const std::vector<const BaseValue *> &iteratables)
	: m_index(index)
	, m_node(node)
	, m_arrayType(arrayType)
	, m_values(iteratables)
{
}

struct K : public fragment
{
	fluenc::expression_t m_subject;

	K(const fluenc::expression_t& subject)
		: m_subject(subject)
	{
	}

	std::vector<DzResult> build(const Emitter& visitor, DefaultVisitorContext context) const override
	{
		return fluenc::accept(m_subject, visitor, context);
	}
};

struct J : public fragment
{
	fluenc::expression_t m_node;

	J(const fluenc::expression_t& node)
		: m_node(node)
	{

	}

	std::vector<DzResult> build(const Emitter& visitor, DefaultVisitorContext context) const override
	{
		UNUSED(visitor);

		auto inputValues = context.values;
		auto tailCallValues = context.entryPoint.values();
		auto iteratorType = context.entryPoint.iteratorType();

		std::vector<const Type *> types = { iteratorType->iteratorType() };

		std::transform(inputValues.begin(), inputValues.end(), back_inserter(types), [](auto value)
		{
			return value->type();
		});

		std::vector<const BaseValue *> cloned;

		std::transform(inputValues.begin(), inputValues.end(), back_inserter(cloned), [&](auto value)
		{
				return value->clone(context.entryPoint, clone_strategy::Value);
		});

		auto next = ValueHelper::extractValues<ExpandedValue>(context.values);

		auto isArray = accumulate(begin(next), end(next), next.size() > 0, [](auto accumulated, auto value)
		{
			return accumulated && value->isArray();
		});

		auto value = new ExpandedValue(isArray
			, expanded_type::get(types)
			, context.entryPoint
			, new K(m_node)
			, next
			, cloned
			);

		return {{ context.entryPoint, value }};
	}
};

std::vector<DzResult> AggregateIteratorValue::build(const Emitter &visitor, DefaultVisitorContext context) const
{
	using values_t = std::vector<std::pair<size_t, Stack>>;
	using results_t = std::map<size_t, std::array<const BaseValue *, 2>>;

	auto llvmContext = context.entryPoint.context();

	auto preEvaluation = new pre_evaluation_node(fluenc::terminator_node::instance());
	auto postEvaluation = new post_evaluation_node();

	values_t valuesByIndex;

	auto preEntryPoint = std::accumulate(fluenc::index_iterator(), fluenc::index_iterator(m_values.size()), context.entryPoint, [&](auto entryPoint, auto index)
	{
		for (auto &[resultEntryPoint, resultValues] : visitor.visit(preEvaluation, { entryPoint, m_values[index] }))
		{
			valuesByIndex.push_back({ index, resultValues });

			return resultEntryPoint;
		}

		return entryPoint;
	});

	results_t resultsByIndex;

	auto postEntryPoint = std::accumulate(begin(valuesByIndex), end(valuesByIndex), preEntryPoint, [&](auto entryPoint, auto pair)
	{
		auto block = llvm::BasicBlock::Create(*llvmContext, "merge");

		auto evaluationResults = visitor.visit(postEvaluation, { entryPoint, pair.second });

		return std::accumulate(begin(evaluationResults), end(evaluationResults), entryPoint, [&](auto, auto result)
		{
			auto &storage = resultsByIndex[result.entryPoint.index()];

			storage[pair.first] = result.values.pop();

			linkBlocks(result.entryPoint.block(), block);

			return result.entryPoint.withBlock(block);
		});
	});

	std::vector<DzResult> results;

        auto arrayContinuation = new ArrayContinuationNode(m_index, this, iterator_type::instance());

	auto lastEntryPoint = std::accumulate(begin(resultsByIndex), prev(end(resultsByIndex)), postEntryPoint, [&](auto entryPoint, auto pair)
	{
		auto [i, values] = pair;

		auto arrayExpandable = new ExpandableValue(true, m_arrayType, entryPoint, arrayContinuation, m_index);

		std::vector<const BaseValue *> elements;
		std::vector<const BaseValue *> dependencies =
		{
			new tuple_value({ arrayExpandable, placeholder_value::instance() })
		};

		// auto continuation = new fluenc::continuation_node(m_node, nullptr);

		auto init = new StaticNode(arrayExpandable
			, new fluenc::expansion_node(fluenc::terminator_node::instance(), nullptr)
			, new J(m_node)
			);

		auto chain = std::accumulate(begin(values), end(values), init, [&](auto next, auto value) -> StaticNode*
		{
			if (!value)
			{
				elements.push_back(without_value::instance());
			}
			else if (auto tuple = value_cast<const tuple_value *>(value))
			{
				dependencies.insert(begin(dependencies), tuple);

				auto tupleElements = tuple->values();

				auto actualValue = tupleElements.pop();

				elements.push_back(actualValue);

				auto expandable = tupleElements.template require<ExpandableValue>(nullptr);

				return new StaticNode(expandable
					, new fluenc::expansion_node(fluenc::terminator_node::instance(), nullptr)
					, next
					);
			}
			else
			{
				elements.push_back(value);
			}

			return next;
		});

		IRBuilderEx builder(entryPoint);

		auto indexLoad = builder.createLoad(m_index, "index");

		auto indexType = int64_type::instance();

		auto storageType = indexType->storageType(*llvmContext);

		auto indexConstant = new scalar_value(indexType
			, llvm::ConstantInt::get(storageType, i)
			);

		auto ifTrue = createBlock(llvmContext);
		auto ifFalse = createBlock(llvmContext);

		auto comparison =  builder.createCmp(llvm::CmpInst::Predicate::ICMP_EQ, indexLoad, indexConstant);

		builder.createCondBr(comparison , ifTrue, ifFalse);

		auto epIfFalse = entryPoint
			.withIndex(i)
			.withBlock(ifFalse);

		auto epIfTrue = entryPoint
			.withIndex(i)
			.withBlock(ifTrue);

		std::vector<const Type *> dependencyTypes =
		{
			m_arrayType
		};

		std::transform(begin(dependencies), end(dependencies), back_inserter(dependencyTypes), [](auto value)
		{
			return value->type();
		});

		auto expandableType = expanded_type::get(dependencyTypes);

		auto expandable = new ExpandableValue(true, expandableType, postEntryPoint, chain, dependencies);

		auto tupleValue = new tuple_value(elements);
		auto finalValue = new tuple_value({ expandable, tupleValue });

		results.push_back({ epIfTrue, finalValue });

		return epIfFalse;
	});

	std::vector<const BaseValue *> elements;

	auto pair = *rbegin(resultsByIndex);

	auto [i, values] = pair;

	std::transform(begin(values), end(values), back_inserter(elements), [](auto value) -> const BaseValue *
	{
		if (!value)
		{
                    return without_value::instance();
		}

		return value;
	});

        results.push_back({ lastEntryPoint.withIndex(i), new tuple_value(elements) });

	return results;
}
