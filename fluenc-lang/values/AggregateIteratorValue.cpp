#include <numeric>

#include <llvm/IR/Constants.h>

#include "AggregateIteratorValue.h"
#include "IndexIterator.h"
#include "IRBuilderEx.h"

#include "nodes/PreEvaluationNode.h"
#include "nodes/PostEvaluationNode.h"
#include "nodes/TerminatorNode.h"
#include "nodes/ArrayContinuationNode.h"
#include "nodes/ContinuationNode.h"
#include "nodes/StaticNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/ExpansionNode.h"

#include "types/IteratorType.h"
#include "types/Int64Type.h"
#include "types/ExpandedType.h"

#include "values/TupleValue.h"
#include "values/ExpandableValue.h"
#include "values/WithoutValue.h"
#include "values/PlaceholderValue.h"
#include "values/ScalarValue.h"
#include "values/ReferenceValue.h"

AggregateIteratorValue::AggregateIteratorValue(const ReferenceValue *index, const ArrayBinaryNode *node, const Type *arrayType, const std::vector<const BaseValue *> &iteratables)
	: m_index(index)
	, m_node(node)
	, m_arrayType(arrayType)
	, m_values(iteratables)
{
}

std::vector<DzResult> AggregateIteratorValue::accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const
{
	using values_t = std::vector<std::pair<size_t, Stack>>;
	using results_t = std::map<size_t, std::array<const BaseValue *, 2>>;

	auto llvmContext = context.entryPoint.context();

	auto preEvaluation = new PreEvaluationNode(TerminatorNode::instance());
	auto postEvaluation = new PostEvaluationNode();

	values_t valuesByIndex;

	auto preEntryPoint = std::accumulate(index_iterator(), index_iterator(m_values.size()), context.entryPoint, [&](auto entryPoint, auto index)
	{
		for (auto &[resultEntryPoint, resultValues] : preEvaluation->accept(visitor, { entryPoint, m_values[index] }))
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

		auto evaluationResults = postEvaluation->accept(visitor, { entryPoint, pair.second });

		return std::accumulate(begin(evaluationResults), end(evaluationResults), entryPoint, [&](auto, auto result)
		{
			auto &storage = resultsByIndex[result.entryPoint.index()];

			storage[pair.first] = result.values.pop();

			linkBlocks(result.entryPoint.block(), block);

			return result.entryPoint.withBlock(block);
		});
	});

	std::vector<DzResult> results;

	auto arrayContinuation = new ArrayContinuationNode(m_index, m_node, IteratorType::instance());

	auto lastEntryPoint = std::accumulate(begin(resultsByIndex), prev(end(resultsByIndex)), postEntryPoint, [&](auto entryPoint, auto pair)
	{
		auto [i, values] = pair;

		auto arrayExpandable = new ExpandableValue(true, m_arrayType, entryPoint, arrayContinuation, m_index);

		std::vector<const BaseValue *> elements;
		std::vector<const BaseValue *> dependencies =
		{
			new TupleValue({ arrayExpandable, PlaceholderValue::instance() })
		};

		auto continuation = new ContinuationNode(m_node, nullptr);

		auto init = new StaticNode(arrayExpandable
			, new ExpansionNode(continuation, nullptr)
			);

		auto chain = std::accumulate(begin(values), end(values), static_cast<const Node *>(init), [&](auto next, auto value) -> const Node *
		{
			if (!value)
			{
				elements.push_back(WithoutValue::instance());
			}
			else if (auto tuple = value_cast<const TupleValue *>(value))
			{
				dependencies.insert(begin(dependencies), tuple);

				auto tupleElements = tuple->values();

				auto actualValue = tupleElements.pop();

				elements.push_back(actualValue);

				auto expandable = tupleElements.template require<ExpandableValue>(m_node->ast);

				return new StaticNode(expandable
					, new ExpansionNode(next, nullptr)
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

		auto indexType = Int64Type::instance();

		auto storageType = indexType->storageType(*llvmContext);

		auto indexConstant = new ScalarValue(indexType
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

		auto expandableType = ExpandedType::get(dependencyTypes);

		auto expandable = new ExpandableValue(true, expandableType, postEntryPoint, chain, dependencies);

		auto tupleValue = new TupleValue(elements);
		auto finalValue = new TupleValue({ expandable, tupleValue });

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
			return WithoutValue::instance();
		}

		return value;
	});

	results.push_back({ lastEntryPoint.withIndex(i), new TupleValue(elements) });

	return results;
}
