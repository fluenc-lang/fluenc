#include "Analyzer.h"
#include "ITypeName.h"
#include "UndeclaredIdentifierException.h"
#include "FunctionNotFoundException.h"
#include "IPrototypeProvider.h"

#include "nodes/BinaryNode.h"
#include "nodes/ExportedFunctionNode.h"
#include "nodes/ArrayContinuationNode.h"
#include "nodes/ArrayElementNode.h"
#include "nodes/IntegralLiteralNode.h"
#include "nodes/FloatLiteralNode.h"
#include "nodes/BooleanLiteralNode.h"
#include "nodes/StringLiteralNode.h"
#include "nodes/CharacterLiteralNode.h"
#include "nodes/NothingNode.h"
#include "nodes/MemberAccessNode.h"
#include "nodes/ReferenceSinkNode.h"
#include "nodes/LazyEvaluationNode.h"
#include "nodes/FunctionCallNode.h"
#include "nodes/StackSegmentNode.h"
#include "nodes/FunctionCallProxyNode.h"
#include "nodes/JunctionNode.h"
#include "nodes/InstantiationNode.h"
#include "nodes/ConditionalNode.h"
#include "nodes/BlockInstructionNode.h"
#include "nodes/EmptyArrayNode.h"
#include "nodes/IndexSinkNode.h"
#include "nodes/ArraySinkNode.h"

#include "types/BooleanType.h"
#include "types/StringType.h"
#include "types/Int32Type.h"
#include "types/WithoutType.h"
#include "types/IPrototype.h"

#include "values/ExpandedValue.h"
#include "values/FunctionValue.h"
#include "values/NamedValue.h"
#include "values/StringValue.h"
#include "values/UserTypeValue.h"
#include "values/ArrayValueGenerator.h"
#include "values/LazyValue.h"
#include "values/TupleValue.h"
#include "values/ForwardedValue.h"
#include "values/IIteratable.h"

#include "exceptions/AmbiguousFunctionException.h"
#include "exceptions/InvalidFunctionPointerTypeException.h"

Analyzer::Analyzer()
{

}

class DummyValueMetadata : public ValueMetadata
{

		// ValueMetadata interface
	public:
		std::string name() const override
		{
			return "type value";
		}
};

class DummyValue : public BaseValueWithMetadata<DummyValueMetadata>
{
	public:
		DummyValue(const Type *type)
			: m_type(type)
		{
		}

		const Type *type() const override
		{
			return m_type;
		}

		const BaseValue *clone(const EntryPoint &entryPoint) const override
		{
			UNUSED(entryPoint);

			return this;
		}

	private:
		const Type *m_type;
};

std::vector<DzResult<BaseValue>> Analyzer::visitBinary(const BinaryNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	auto left = values.pop();
	auto right = values.pop();

	auto leftType = left->type();
	auto rightType = right->type();

	if (leftType->compatibility(rightType, entryPoint) > 0)
	{
		throw new std::exception(); // TODO
	}

	values.push(left);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> Analyzer::visitExportedFunction(const ExportedFunctionNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return node->m_block->accept(*this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> Analyzer::visitArrayContinuation(const ArrayContinuationNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(values);

	return {{ entryPoint, new DummyValue(node->m_iteratorType) }};
}

std::vector<DzResult<BaseValue> > Analyzer::visitArrayElement(const ArrayElementNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	if (node->m_next)
	{
		return node->m_next->accept(*this, entryPoint, values);
	}

	return {{ entryPoint, values }};
}

std::vector<DzResult<BaseValue> > Analyzer::visitIntegralLiteral(const IntegralLiteralNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	auto type = node->m_type->resolve(entryPoint);

	values.push(new DummyValue(type));

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult<BaseValue> > Analyzer::visitFloatLiteral(const FloatLiteralNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	auto type = node->m_type->resolve(entryPoint);

	values.push(new DummyValue(type));

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult<BaseValue> > Analyzer::visitBooleanLiteral(const BooleanLiteralNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	values.push(new DummyValue(BooleanType::instance()));

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult<BaseValue> > Analyzer::visitStringLiteral(const StringLiteralNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	values.push(new DummyValue(StringType::get(node->m_value.size())));

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult<BaseValue> > Analyzer::visitCharacterLiteral(const CharacterLiteralNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	values.push(new DummyValue(Int32Type::instance()));

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult<BaseValue> > Analyzer::visitNothing(const NothingNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	values.push(new DummyValue(WithoutType::instance()));

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult<BaseValue> > Analyzer::visitMemberAccess(const MemberAccessNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	auto locals = entryPoint.locals();
	auto functions = entryPoint.functions();
	auto globals = entryPoint.globals();

	for (auto &name : node->m_names)
	{
		auto localsIterator = locals.find(name);

		if (localsIterator != locals.end())
		{
			values.push(localsIterator->second);

			return node->m_consumer->accept(*this, entryPoint, values);
		}

		auto functionsIterator = functions.find(name);

		if (functionsIterator != functions.end())
		{
			auto value = new FunctionValue(functionsIterator->second, entryPoint);

			values.push(value);

			return node->m_consumer->accept(*this, entryPoint, values);
		}

		auto globalsIterator = globals.find(name);

		if (globalsIterator != globals.end())
		{
			std::vector<DzResult<BaseValue>> results;

			for (auto &[resultEntryPoint, resultValues] : globalsIterator->second->accept(*this, entryPoint, values))
			{
				for (auto &result : node->m_consumer->accept(*this, resultEntryPoint, resultValues))
				{
					results.push_back(result);
				}
			}

			return results;
		}
	}

	throw new UndeclaredIdentifierException(node->m_ast, node->m_names[0]);
}

std::vector<DzResult<BaseValue> > Analyzer::visitReferenceSink(const ReferenceSinkNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult<BaseValue> > Analyzer::visitLazyEvaluation(const LazyEvaluationNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	auto digestDepth = [this](const EntryPoint &entryPoint, Stack<BaseValue> values, auto &recurse) -> std::vector<DzResult<BaseValue>>
	{
		for (auto i = 0u; i < values.size(); i++)
		{
			auto value = values.pop();

			if (auto forwarded = dynamic_cast<const ForwardedValue *>(value))
			{
				std::vector<DzResult<BaseValue>> results;

				for (auto &[resultEntryPoint, resultValues] : recurse(entryPoint, values, recurse))
				{
					std::vector<const BaseValue *> forwardedValues;

					for (auto resultValue : resultValues)
					{
						forwardedValues.push_back(resultValue);
					}

					auto subject = forwarded->subject();

					forwardedValues.push_back(subject);

					results.push_back({ resultEntryPoint, forwardedValues });
				}

				return results;
			}

			if (auto lazy = dynamic_cast<const LazyValue *>(value))
			{
				auto iteratable = lazy->generate(entryPoint);

				std::vector<DzResult<BaseValue>> results;

				for (auto &[resultEntryPoint, resultValues] : iteratable->accept(*this, entryPoint, Stack<BaseValue>()))
				{
					auto forwardedValues = values;

					for (auto &resultValue : resultValues)
					{
						forwardedValues.push(resultValue);
					}

					auto forwardedEntryPoint = resultEntryPoint
						.withIteratorStorage(entryPoint.iteratorStorage());

					for (auto &result : recurse(forwardedEntryPoint, forwardedValues, recurse))
					{
						results.push_back(result);
					}
				}

				return results;
			}

			if (auto string = dynamic_cast<const StringValue *>(value))
			{
				auto iterator = string->iterator();

				auto forwardedValues = values;

				forwardedValues.push(iterator);

				return recurse(entryPoint, forwardedValues, recurse);
			}

			if (auto tuple = dynamic_cast<const TupleValue *>(value))
			{
				for (auto &element : tuple->values())
				{
					if (auto expanded = dynamic_cast<const ExpandedValue *>(element))
					{
						auto node = expanded->node();

						std::vector<DzResult<BaseValue>> results;

						for (auto &[resultEntryPoint, resultValues] : node->accept(*this, entryPoint, values))
						{
							auto forwardedValues = values;

							for (auto &resultValue : resultValues)
							{
								forwardedValues.push(resultValue);
							}

							auto forwardedEntryPoint = resultEntryPoint
								.withIteratorStorage(entryPoint.iteratorStorage());

							for (auto &result : recurse(forwardedEntryPoint, forwardedValues, recurse))
							{
								results.push_back(result);
							}
						}

						return results;
					}
				}

				std::vector<DzResult<BaseValue>> results;

				auto digestedResults = recurse(entryPoint
					, tuple->values()
					, recurse
					);

				for (auto &[digestedEntryPoint, digestedValues] : digestedResults)
				{
					auto digestedTuple = new TupleValue({ digestedValues.begin(), digestedValues.end() });

					for (auto &[resultEntryPoint, resultValues] : recurse(digestedEntryPoint, values, recurse))
					{
						std::vector<const BaseValue *> forwardedValues;

						for (auto resultValue : resultValues)
						{
							forwardedValues.push_back(resultValue);
						}

						forwardedValues.push_back(digestedTuple);

						results.push_back({ resultEntryPoint, forwardedValues });
					}
				}

				return results;
			}

			std::vector<DzResult<BaseValue>> results;

			for (auto &[resultEntryPoint, resultValues] : recurse(entryPoint, values, recurse))
			{
				std::vector<const BaseValue *> forwardedValues;

				for (auto resultValue : resultValues)
				{
					forwardedValues.push_back(resultValue);
				}

				forwardedValues.push_back(value);

				results.push_back({ resultEntryPoint, forwardedValues });
			}

			return results;
		}

		return {{ entryPoint, values }};
	};

	std::vector<DzResult<BaseValue>> results;

	for (auto &[resultEntryPoint, resultValues] : digestDepth(entryPoint, values, digestDepth))
	{
		for (auto &result : node->m_consumer->accept(*this, resultEntryPoint, resultValues))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult<BaseValue> > Analyzer::visitFunctionCall(const FunctionCallNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	auto findFunction = [&](const std::vector<const Type *> &types) -> const CallableNode *
	{
		auto functions = entryPoint.functions();
		auto locals = entryPoint.locals();

		for (auto &name : node->m_names)
		{
			auto local = locals.find(name);

			if (local != locals.end())
			{
				auto value = dynamic_cast<const FunctionValue *>(local->second);

				if (!value)
				{
					throw new InvalidFunctionPointerTypeException(node->m_ast, name);
				}

				return value->function();
			}

			std::map<int8_t, CallableNode *> candidates;

			for (auto [i, end] = functions.equal_range(name); i != end; i++)
			{
				auto function = i->second;

				auto score = function->signatureCompatibility(entryPoint, types);

				if (score < 0)
				{
					continue;
				}

				auto candidate = candidates.find(score);

				if (candidate != candidates.end())
				{
					std::vector<CallableNode *> functions = { candidate->second, function };

					throw new AmbiguousFunctionException(node->m_ast
						, functions
						, entryPoint
						);
				}

				candidates[score] = function;
			}

			if (candidates.size() > 0)
			{
				auto [_, function] = *candidates.begin();

				return function;
			}
		}

		return nullptr;
	};

	std::vector<DzResult<BaseValue>> result;

	for (auto &[resultEntryPoint, resultValues] : node->m_evaluation->accept(*this, entryPoint, values))
	{
		auto locals = resultEntryPoint.locals();

		std::vector<const Type *> types;

		std::transform(resultValues.rbegin(), resultValues.rend(), std::back_inserter(types), [](auto value)
		{
			return value->type();
		});

		auto function = findFunction(types);

		if (!function)
		{
			throw new FunctionNotFoundException(node->m_ast, node->m_names[0], types);
		}

		auto functionResults = function->accept(*this, resultEntryPoint, resultValues);

		for (const auto &[lastEntryPoint, returnValue] : functionResults)
		{
			result.push_back({ lastEntryPoint, returnValue });
		}
	}

	return result;
}

std::vector<DzResult<BaseValue> > Analyzer::visitStackSegment(const StackSegmentNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	std::vector<DzResult<BaseValue>> result;
	std::vector<DzResult<BaseValue>> input = {{ entryPoint, Stack<BaseValue>() }};

	auto subjectResults = std::accumulate(begin(node->m_values), end(node->m_values), input, [&](auto accumulator, auto argument)
	{
		std::vector<DzResult<BaseValue>> results;

		for (auto &[accumulatorEntryPoint, accumulatorValues] : accumulator)
		{
			auto result = argument->accept(*this, accumulatorEntryPoint, Stack<BaseValue>());

			for (auto &[resultEntryPoint, resultValues] : result)
			{
				auto scopedReturnValues = accumulatorValues;

				for (auto resultValue : resultValues)
				{
					scopedReturnValues.push(resultValue);
				}

				results.push_back({ resultEntryPoint, scopedReturnValues });
			}
		}

		return results;
	});

	for (auto &[subjectEntryPoint, subjectValues] : subjectResults)
	{
		auto callResults = node->m_call->accept(*this, subjectEntryPoint, subjectValues);

		for (auto &[callEntryPoint, callValues] : callResults)
		{
			auto forwardedValues = values;

			for (auto &value : callValues)
			{
				forwardedValues.push(value);
			}

			auto consumerResults = node->m_consumer->accept(*this, callEntryPoint, forwardedValues);

			for (auto &consumerResult : consumerResults)
			{
				result.push_back(consumerResult);
			}
		}
	}

	return result;
}

std::vector<DzResult<BaseValue> > Analyzer::visitFunctionCallProxy(const FunctionCallProxyNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	std::vector<DzResult<BaseValue>> results;

	for (auto &[resultEntryPoint, resultValues] : node->m_regularCall->accept(*this, entryPoint, values))
	{
		for (auto &result : node->m_consumer->accept(*this, resultEntryPoint, resultValues))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult<BaseValue> > Analyzer::visitJunction(const JunctionNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return node->m_subject->accept(*this, entryPoint, values);
}

std::vector<DzResult<BaseValue> > Analyzer::visitInstantiation(const InstantiationNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	std::unordered_map<std::string, const BaseValue *> valuesByName;

	std::transform(begin(node->m_fields), end(node->m_fields), std::inserter(valuesByName, begin(valuesByName)), [&](auto field)
	{
		return std::make_pair(field, values.pop());
	});

	auto prototype = node->m_prototypeProvider->provide(entryPoint, values);

	auto prototypeFields = prototype->fields(entryPoint);

	std::vector<const NamedValue *> namedValues;

	std::transform(begin(prototypeFields), end(prototypeFields), std::back_inserter(namedValues), [&](auto field) -> const NamedValue *
	{
		auto valueByName = valuesByName.find(field.name());

		if (valueByName != valuesByName.end())
		{
			return new NamedValue { field.name(), valueByName->second };
		}

		return new NamedValue { field.name(), field.defaultValue() };
	});

	auto userTypeValue = new UserTypeValue(prototype, namedValues);

	values.push(userTypeValue);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult<BaseValue> > Analyzer::visitConditional(const ConditionalNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	auto resultsIfTrue = node->m_ifTrue->accept(*this, entryPoint, values);
	auto resultsIfFalse = node->m_ifFalse->accept(*this, entryPoint, values);

	std::vector<DzResult<BaseValue>> results;

	results.insert(end(results), begin(resultsIfTrue), end(resultsIfTrue));
	results.insert(end(results), begin(resultsIfFalse), end(resultsIfFalse));

	return results;
}

std::vector<DzResult<BaseValue> > Analyzer::visitBlockInstruction(const BlockInstructionNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return node->m_subject->accept(*this, entryPoint, values);
}

std::vector<DzResult<BaseValue> > Analyzer::visitEmptyArray(const EmptyArrayNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	values.push(new DummyValue(WithoutType::instance()));

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult<BaseValue> > Analyzer::visitIndexSink(const IndexSinkNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult<BaseValue> > Analyzer::visitArraySink(const ArraySinkNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	auto arrayContents = node->m_firstValue->accept(*this, entryPoint, Stack<BaseValue>());

	auto generator = new ArrayValueGenerator(arrayContents, node->id(), node->m_size);
	auto lazy = new LazyValue(generator, entryPoint);

	values.push(lazy);

	std::vector<DzResult<BaseValue>> results;

	for (auto &[arrayEntryPoint, _] : arrayContents)
	{
		for (auto &result : node->m_consumer->accept(*this, arrayEntryPoint, values))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult<BaseValue> > Analyzer::visitExpansion(const ExpansionNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult<BaseValue>>();
}

std::vector<DzResult<BaseValue> > Analyzer::visitLocal(const LocalNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult<BaseValue>>();
}

std::vector<DzResult<BaseValue> > Analyzer::visitContinuation(const ContinuationNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult<BaseValue>>();
}

std::vector<DzResult<BaseValue> > Analyzer::visitUnary(const UnaryNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult<BaseValue>>();
}

std::vector<DzResult<BaseValue> > Analyzer::visitTailFunctionCall(const TailFunctionCallNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult<BaseValue>>();
}

std::vector<DzResult<BaseValue> > Analyzer::visitFunction(const FunctionNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult<BaseValue>>();
}

std::vector<DzResult<BaseValue> > Analyzer::visitExportedFunctionTerminator(const ExportedFunctionTerminatorNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult<BaseValue>>();
}

std::vector<DzResult<BaseValue> > Analyzer::visitImportedFunction(const ImportedFunctionNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult<BaseValue>>();
}

std::vector<DzResult<BaseValue> > Analyzer::visitGlobal(const GlobalNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult<BaseValue>>();
}

std::vector<DzResult<BaseValue> > Analyzer::visitReturn(const ReturnNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult<BaseValue>>();
}

std::vector<DzResult<BaseValue> > Analyzer::visitParentInjector(const ParentInjectorNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult<BaseValue>>();
}

std::vector<DzResult<BaseValue> > Analyzer::visitBlockStackFrame(const BlockStackFrameNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult<BaseValue>>();
}

std::vector<DzResult<BaseValue> > Analyzer::visitTerminator(const TerminatorNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult<BaseValue>>();
}

std::vector<DzResult<BaseValue> > Analyzer::visitIteratable(const IteratableNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult<BaseValue>>();
}

std::vector<DzResult<BaseValue> > Analyzer::visitArrayValue(const ArrayValue *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult<BaseValue>>();
}

std::vector<DzResult<BaseValue> > Analyzer::visitIteratorValue(const IteratorValue *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult<BaseValue>>();
}

std::vector<DzResult<BaseValue> > Analyzer::visitStringIteratable(const StringIteratable *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult<BaseValue>>();
}
