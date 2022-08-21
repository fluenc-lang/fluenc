#include <llvm/IR/Verifier.h>

#include "Analyzer.h"
#include "FunctionNotFoundException.h"
#include "ValueHelper.h"
#include "ITypeName.h"
#include "UndeclaredIdentifierException.h"
#include "IteratorStorage.h"
#include "FunctionHelper.h"
#include "IPrototypeProvider.h"
#include "DzArgument.h"
#include "DzTupleArgument.h"
#include "DummyIteratorStorage.h"

#include "types/IOperatorSet.h"
#include "types/BooleanType.h"
#include "types/StringType.h"
#include "types/Int32Type.h"
#include "types/IPrototype.h"
#include "types/VoidType.h"

#include "values/ScalarValue.h"
#include "values/ExpandedValue.h"
#include "values/ExpandableValue.h"
#include "values/IndexedValue.h"
#include "values/TupleValue.h"
#include "values/ReferenceValue.h"
#include "values/StringValue.h"
#include "values/WithoutValue.h"
#include "values/FunctionValue.h"
#include "values/UserTypeValue.h"
#include "values/NamedValue.h"
#include "values/ForwardedValue.h"
#include "values/LazyValue.h"
#include "values/IIteratable.h"
#include "values/IteratorValueGenerator.h"
#include "values/ArrayValueGenerator.h"
#include "values/PlaceholderValue.h"
#include "values/ArrayValue.h"
#include "values/IteratorValue.h"
#include "values/StringIteratable.h"

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
#include "nodes/ExpansionNode.h"
#include "nodes/LocalNode.h"
#include "nodes/ContinuationNode.h"
#include "nodes/UnaryNode.h"
#include "nodes/TailFunctionCallNode.h"
#include "nodes/FunctionNode.h"
#include "nodes/ImportedFunctionNode.h"
#include "nodes/GlobalNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/ParentInjectorNode.h"
#include "nodes/BlockStackFrameNode.h"
#include "nodes/IteratableNode.h"

class DummyValueMetadata : public ValueMetadata
{
	public:
		std::string name() const
		{
			return "dummy value";
		}
};

class DummyValue : public BaseValueWithMetadata<DummyValueMetadata>
{
	public:
		DummyValue(const Type *type)
			: m_type(type)
		{
		}

		const Type *type() const
		{
			return m_type;
		}

		const BaseValue *clone(const EntryPoint &entryPoint) const
		{
			UNUSED(entryPoint);

			return this;
		}

	private:
		const Type *m_type;
};

std::vector<DzResult> Analyzer::visitBooleanBinary(const BinaryNode *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(node);

	return {{ entryPoint, values.discard() }};
}

std::vector<DzResult> Analyzer::visitFloatBinary(const BinaryNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto left = values.pop();
	auto right = values.pop();

	UNUSED(right);

	auto valueFactory = [&]() -> const BaseValue *
	{
		if (node->m_op == "+")
		{
			return left;
		}

		if (node->m_op == "-")
		{
			return left;
		}

		if (node->m_op == "*")
		{
			return left;
		}

		if (node->m_op == "/")
		{
			return left;
		}

		if (node->m_op == "<")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->m_op == "<=")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->m_op == ">")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->m_op == ">=")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->m_op == "==")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->m_op == "!=")
		{
			return new DummyValue(BooleanType::instance());
		}

		throw new std::exception();
	};

	auto value = valueFactory();

	values.push(value);

	return {{ entryPoint, values }};
}

std::vector<DzResult> Analyzer::visitIntegerBinary(const BinaryNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto left = values.pop();
	auto right = values.pop();

	UNUSED(right);

	auto valueFactory = [&]() -> const BaseValue *
	{
		if (node->m_op == "+")
		{
			return left;
		}

		if (node->m_op == "-")
		{
			return left;
		}

		if (node->m_op == "*")
		{
			return left;
		}

		if (node->m_op == "/")
		{
			return left;
		}

		if (node->m_op == "<")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->m_op == "<=")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->m_op == ">")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->m_op == ">=")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->m_op == "==")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->m_op == "!=")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->m_op == "%")
		{
			return left;
		}

		if (node->m_op == "|")
		{
			return left;
		}

		if (node->m_op == "&")
		{
			return left;
		}

		if (node->m_op == "^")
		{
			return left;
		}

		throw new std::exception();
	};

	auto value = valueFactory();

	values.push(value);

	return {{ entryPoint, values }};
}

std::vector<DzResult> Analyzer::visitBinary(const BinaryNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto left = values.peek();

	auto leftType = left->type();
	auto operators = leftType->operators();

	auto binary = operators->forBinary(node);

	std::vector<DzResult> results;

	for (auto &[resultEntryPoint, resultValues] : binary->accept(*this, entryPoint, values))
	{
		for (auto &result : node->m_consumer->accept(*this, resultEntryPoint, resultValues))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Analyzer::visitExportedFunction(const ExportedFunctionNode *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult>();
}

std::vector<DzResult> Analyzer::visitArrayContinuation(const ArrayContinuationNode *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	auto value = new ExpandedValue(true
		, node->m_iteratorType
		, entryPoint
		, node->m_node
		, node
		, std::vector<const ExpandedValue *>()
		);

	return {{ entryPoint, value }};
}

std::vector<DzResult> Analyzer::visitArrayElement(const ArrayElementNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto value = values.require<IndexedValue>(nullptr);

	auto ep = entryPoint
		.withIndex(value->index());

	if (node->m_next)
	{
		auto continuation = new ArrayContinuationNode(nullptr, node->m_node, IteratorType::instance());
		auto expandable = new ExpandableValue(true, node->m_arrayType, entryPoint, continuation);

		auto tuple = new TupleValue({ expandable, value->subject() });

		auto resultsIfFalse = node->m_next->accept(*this, ep, values);

		std::vector<DzResult> result = {{ ep, tuple }};

		result.insert(end(result), begin(resultsIfFalse), end(resultsIfFalse));

		return result;
	}

	values.push(value->subject());

	return {{ ep, values }};
}

std::vector<DzResult> Analyzer::visitIntegralLiteral(const IntegralLiteralNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto context = entryPoint.context();

	auto type = node->m_type->resolve(entryPoint);
	auto storageType = type->storageType(*context);

	auto valueProvider = [&]
	{
		auto hex = node->m_value.find("0x");

		if (hex != std::string::npos)
		{
			return llvm::ConstantInt::get((llvm::IntegerType *)storageType, node->m_value.substr(hex + 2), 16);
		}

		return llvm::ConstantInt::get((llvm::IntegerType *)storageType, node->m_value, 10);
	};

	auto value = new ScalarValue(type
		, valueProvider()
		);

	values.push(value);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Analyzer::visitFloatLiteral(const FloatLiteralNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto context = entryPoint.context();

	auto type = node->m_type->resolve(entryPoint);
	auto storageType = type->storageType(*context);

	auto value = new ScalarValue(type
		, llvm::ConstantFP::get(storageType, node->m_value)
		);

	values.push(value);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Analyzer::visitBooleanLiteral(const BooleanLiteralNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto context = entryPoint.context();

	auto valueProvider = [&]
	{
		auto type = BooleanType::instance();
		auto storageType = type->storageType(*context);

		if (node->m_value == "true")
		{
			return new ScalarValue { type, llvm::ConstantInt::get(storageType, 1) };
		}

		if (node->m_value == "false")
		{
			return new ScalarValue { type, llvm::ConstantInt::get(storageType, 0) };
		}

		throw new std::exception(); // TODO
	};

	auto value = valueProvider();

	values.push(value);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Analyzer::visitStringLiteral(const StringLiteralNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto stringType = StringType::get(node->m_value.size());

	auto string = new DummyValue(stringType);

	values.push(string);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Analyzer::visitCharacterLiteral(const CharacterLiteralNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto type = Int32Type::instance();

	auto value = new DummyValue(type);

	values.push(value);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Analyzer::visitNothing(const NothingNode *node, const EntryPoint &entryPoint, Stack values) const
{
	values.push(WithoutValue::instance());

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Analyzer::visitMemberAccess(const MemberAccessNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto &locals = entryPoint.locals();
	auto &functions = entryPoint.functions();
	auto &globals = entryPoint.globals();

	for (auto &name : node->m_names)
	{
		auto localsIterator = locals.find(name);

		if (localsIterator != locals.end())
		{
			auto forwarded = localsIterator->second->forward(node->id());

			values.push(forwarded);

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
			std::vector<DzResult> results;

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

std::vector<DzResult> Analyzer::visitReferenceSink(const ReferenceSinkNode *node, const EntryPoint &entryPoint, Stack values) const
{
	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Analyzer::visitLazyEvaluation(const LazyEvaluationNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto digestDepth = [this](const EntryPoint &entryPoint, Stack values, auto &recurse) -> std::vector<DzResult>
	{
		for (auto i = 0u; i < values.size(); i++)
		{
			auto value = values.pop();

			if (auto forwarded = dynamic_cast<const ForwardedValue *>(value))
			{
				std::vector<DzResult> results;

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

				std::vector<DzResult> results;

				for (auto &[resultEntryPoint, resultValues] : iteratable->accept(*this, entryPoint, Stack()))
				{
					auto forwardedValues = values;

					for (auto &resultValue : resultValues)
					{
						forwardedValues.push(resultValue);
					}

					for (auto &result : recurse(resultEntryPoint, forwardedValues, recurse))
					{
						results.push_back(result);
					}
				}

				return results;
			}

			if (auto string = dynamic_cast<const StringValue *>(value))
			{
				auto iterator =	string->iterator();

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

						std::vector<DzResult> results;

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

				std::vector<DzResult> results;

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

			std::vector<DzResult> results;

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

	auto ep = entryPoint
		.withIteratorStorage(new DummyIteratorStorage());

	std::vector<DzResult> results;

	for (auto &[resultEntryPoint, resultValues] : digestDepth(ep, values, digestDepth))
	{
		for (auto &result : node->m_consumer->accept(*this, resultEntryPoint, resultValues))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Analyzer::visitFunctionCall(const FunctionCallNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto findFunction = [&](const std::vector<const Type *> &types) -> const CallableNode *
	{
		auto &functions = entryPoint.functions();
		auto &locals = entryPoint.locals();

		for (auto &name : node->m_names)
		{
			auto local = locals.find(name);

			if (local != locals.end())
			{
				auto value = dynamic_cast<const FunctionValue *>(local->second);

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

	std::vector<DzResult> result;

	for (auto &[resultEntryPoint, resultValues] : node->m_evaluation->accept(*this, entryPoint, values))
	{
		auto &locals = resultEntryPoint.locals();

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

		if (function->attribute() == FunctionAttribute::Import)
		{
			return function->accept(*this, resultEntryPoint, resultValues);
		}

		auto functionResults = function->accept(*this, resultEntryPoint, resultValues);

		for (const auto &[lastEntryPoint, returnValue] : functionResults)
		{
			auto consumerEntryPoint = resultEntryPoint
				.withDepth(lastEntryPoint.depth());

			result.push_back({ consumerEntryPoint, returnValue });
		}
	}

	return result;
}

std::vector<DzResult> Analyzer::visitStackSegment(const StackSegmentNode *node, const EntryPoint &entryPoint, Stack values) const
{
	std::vector<DzResult> result;
	std::vector<DzResult> input = {{ entryPoint, Stack() }};

	auto subjectResults = std::accumulate(rbegin(node->m_values), rend(node->m_values), input, [&](auto accumulator, auto argument)
	{
		std::vector<DzResult> results;

		for (auto &[accumulatorEntryPoint, accumulatorValues] : accumulator)
		{
			auto result = argument->accept(*this, accumulatorEntryPoint, Stack());

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

std::vector<DzResult> Analyzer::visitFunctionCallProxy(const FunctionCallProxyNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto &functions = entryPoint.functions();

	for (auto &name : node->m_names)
	{
		for (auto [i, end] = functions.equal_range(name); i != end; i++)
		{
			auto function = i->second;

			// Naive. Really naive.
			if (function->attribute() == FunctionAttribute::Iterator)
			{
				auto generator = new IteratorValueGenerator(new IteratorType(), node->m_regularCall, entryPoint);
				auto lazy = new LazyValue(generator, entryPoint);

				auto forwardedValues = values;

				forwardedValues.push(lazy);

				std::vector<DzResult> results;

				for (auto &result : node->m_consumer->accept(*this, entryPoint, forwardedValues))
				{
					results.push_back(result);
				}

				return results;
			}
		}
	}

	std::vector<DzResult> results;

	for (auto &[resultEntryPoint, resultValues] : node->m_regularCall->accept(*this, entryPoint, values))
	{
		for (auto &result : node->m_consumer->accept(*this, resultEntryPoint, resultValues))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Analyzer::visitJunction(const JunctionNode *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult>();
}

std::vector<DzResult> Analyzer::visitInstantiation(const InstantiationNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto module = entryPoint.module();

	auto dataLayout = module->getDataLayout();

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
			auto value = valueByName->second;

			valuesByName.erase(valueByName);

			return new NamedValue { field.name(), value };
		}

		return new NamedValue { field.name(), field.defaultValue() };
	});

	auto userTypeValue = new UserTypeValue(prototype, namedValues);

	values.push(userTypeValue);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Analyzer::visitConditional(const ConditionalNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto condition = values.pop();

	UNUSED(condition);

	auto resultsIfTrue = node->m_ifTrue->accept(*this, entryPoint, values);
	auto resultsIfFalse = node->m_ifFalse->accept(*this, entryPoint, values);

	std::vector<DzResult> immediateResults;

	immediateResults.insert(end(immediateResults), begin(resultsIfTrue), end(resultsIfTrue));
	immediateResults.insert(end(immediateResults), begin(resultsIfFalse), end(resultsIfFalse));

	return immediateResults;
}

std::vector<DzResult> Analyzer::visitBlockInstruction(const BlockInstructionNode *node, const EntryPoint &entryPoint, Stack values) const
{
	return node->m_subject->accept(*this, entryPoint, values);
}

std::vector<DzResult> Analyzer::visitEmptyArray(const EmptyArrayNode *node, const EntryPoint &entryPoint, Stack values) const
{
	values.push(WithoutValue::instance());

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Analyzer::visitIndexSink(const IndexSinkNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto value = values.pop();

	auto indexed = new IndexedValue(node->m_index, value);

	values.push(indexed);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Analyzer::visitArraySink(const ArraySinkNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto arrayContents = node->m_firstValue->accept(*this, entryPoint, Stack());

	auto generator = new ArrayValueGenerator(arrayContents, node->id(), node->m_size);
	auto lazy = new LazyValue(generator, entryPoint);

	values.push(lazy);

	std::vector<DzResult> results;

	for (auto &[arrayEntryPoint, _] : arrayContents)
	{
		for (auto &result : node->m_consumer->accept(*this, arrayEntryPoint, values))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Analyzer::visitExpansion(const ExpansionNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto expandable = values.require<ExpandableValue>(node->m_ast);

	auto continuation = expandable->chain();
	auto provider = expandable->provider();

	auto result = continuation->accept(*this, *provider, Stack());

	for (auto &[targetEntryPoint, continuationValues] : result)
	{
		auto value = continuationValues
			.require<ExpandedValue>(nullptr);

		auto tuple = new TupleValue({ value, PlaceholderValue::instance() });

		values.push(tuple);

		return node->m_consumer->accept(*this, entryPoint, values);
	}

	throw new std::exception();
}

std::vector<DzResult> Analyzer::visitLocal(const LocalNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto locals = entryPoint.locals();

	auto value = values.pop();

	if (auto userValue = dynamic_cast<const UserTypeValue * >(value))
	{
		auto fields = userValue->fields();

		std::transform(begin(fields), end(fields), std::inserter(locals, begin(locals)), [=](auto field) -> std::pair<std::string, const BaseValue *>
		{
			std::stringstream ss;
			ss << node->m_name;
			ss << ".";
			ss << field->name();

			return { ss.str(), field->value() };
		});
	}

	locals[node->m_name] = value;

	auto ep = entryPoint
		.withLocals(locals);

	return node->m_consumer->accept(*this, ep, values);
}

std::vector<DzResult> Analyzer::visitContinuation(const ContinuationNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto next = ValueHelper::extractValues<ExpandedValue>(values);

	auto isArray = accumulate(begin(next), end(next), next.size() > 0, [](auto accumulated, auto value)
	{
		return accumulated && value->isArray();
	});

	auto value = new ExpandedValue(isArray
		, node->m_iteratorType
		, entryPoint
		, node->m_node
		, node
		, next
		);

	return {{ entryPoint, value }};
}

std::vector<DzResult> Analyzer::visitUnary(const UnaryNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto operand = values.pop();

	auto resolveOp = [&]
	{
		if (node->m_op == "@")
		{
			return new ForwardedValue(operand);
		}

		throw new std::exception(); // TODO
	};

	auto value = resolveOp();

	values.push(value);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Analyzer::visitTailFunctionCall(const TailFunctionCallNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto [score, tailCallTarget, targetValues] = FunctionHelper::tryCreateTailCall(entryPoint, values, begin(node->m_names), end(node->m_names));

	if (score == 0)
	{
		return std::vector<DzResult>();
	}

	if (score == 1)
	{
		throw new std::exception(); // TODO
	}

	return node->m_regularCall->accept(*this, entryPoint, values);
}

std::vector<DzResult> Analyzer::visitFunction(const FunctionNode *node, const EntryPoint &entryPoint, Stack values) const
{
	struct Argument
	{
		std::string name;

		const BaseValue *value;
	};

	auto handleArgument = [&](DzBaseArgument *argument, const BaseValue *value, auto &recurse)
	{
		if (auto standardArgument = dynamic_cast<DzArgument *>(argument))
		{
			auto name = standardArgument->name();

			std::vector<Argument> result
			{
				{ name, value }
			};

			if (auto userValue = dynamic_cast<const UserTypeValue * >(value))
			{
				auto fields = userValue->fields();

				std::transform(begin(fields), end(fields), std::back_inserter(result), [=](auto field) -> Argument
				{
					std::stringstream ss;
					ss << name;
					ss << ".";
					ss << field->name();

					return { ss.str(), field->value() };
				});
			}

			return result;
		}

		if (auto tupleArgument = dynamic_cast<DzTupleArgument *>(argument))
		{
			auto tupleValue = dynamic_cast<const TupleValue *>(value);

			auto tupleValues = tupleValue->values();
			auto arguments = tupleArgument->arguments();

			std::vector<Argument> results;

			for (auto argument : arguments)
			{
				for (auto &result : recurse(argument, tupleValues.pop(), recurse))
				{
					results.push_back(result);
				}
			}

			return results;
		}

		throw new std::exception();
	};

	auto pep = entryPoint
		.withValues(values);

	std::map<std::string, const BaseValue *> locals;

	for (const auto &argument : node->m_arguments)
	{
		for (auto &[name, value] : handleArgument(argument, values.pop(), handleArgument))
		{
			locals[name] = value;
		}
	}

	auto ep = pep
		.withName(node->m_name)
		.markEntry()
		.withLocals(locals)
		.withIteratorStorage(nullptr);

	return node->m_block->accept(*this, ep, values);
}

std::vector<DzResult> Analyzer::visitExportedFunctionTerminator(const ExportedFunctionTerminatorNode *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(node);
	UNUSED(entryPoint);
	UNUSED(values);

	return std::vector<DzResult>();
}

std::vector<DzResult> Analyzer::visitImportedFunction(const ImportedFunctionNode *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	auto returnType = node->m_returnType->resolve(entryPoint);

	if (returnType != VoidType::instance())
	{
		return {{ entryPoint, new DummyValue(returnType) }};
	}

	return {{ entryPoint, Stack() }};
}

std::vector<DzResult> Analyzer::visitGlobal(const GlobalNode *node, const EntryPoint &entryPoint, Stack values) const
{
	return node->m_value->accept(*this, entryPoint, values);
}

std::vector<DzResult> Analyzer::visitReturn(const ReturnNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto value = values.pop();

	if (node->m_chained)
	{
		auto expandable = new ExpandableValue(false, node->m_iteratorType, entryPoint, node->m_chained);
		auto tuple = new TupleValue({ expandable, value });

		values.push(tuple);

		return node->m_consumer->accept(*this, entryPoint, values);
	}

	values.push(value);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Analyzer::visitParentInjector(const ParentInjectorNode *node, const EntryPoint &entryPoint, Stack values) const
{
	return node->m_subject->accept(*this, entryPoint, values);
}

std::vector<DzResult> Analyzer::visitBlockStackFrame(const BlockStackFrameNode *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	return node->m_consumer->accept(*this, entryPoint, Stack());
}

std::vector<DzResult> Analyzer::visitTerminator(const TerminatorNode *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(node);

	return {{ entryPoint, values }};
}

std::vector<DzResult> Analyzer::visitIteratable(const IteratableNode *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	return node->m_iteratable->accept(*this, entryPoint, values);
}

std::vector<DzResult> Analyzer::visitArrayValue(const ArrayValue *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	std::vector<DzResult> results;

	for (auto [_, elementValues] : node->m_values)
	{
		auto iteratorEntryPoint = entryPoint
			.withName("__array")
			.markEntry()
			;

		for (auto &result : node->m_iterator->accept(*this, iteratorEntryPoint, elementValues))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Analyzer::visitIteratorValue(const IteratorValue *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	auto locals = entryPoint.locals();

	for (auto &[key, value] : node->m_entryPoint->locals())
	{
		locals[key] = value;
	}

	auto ep = (*node->m_entryPoint)
		.withLocals(locals)
		.withIteratorStorage(entryPoint.iteratorStorage());

	return node->m_subject->accept(*this, ep, Stack());
}

std::vector<DzResult> Analyzer::visitStringIteratable(const StringIteratable *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	auto iteratorEntryPoint = entryPoint
		.withName("__iterator")
		.markEntry();

	auto value = new DummyValue(Int32Type::instance());

	auto continuation = new ArrayContinuationNode(node->m_index, node->m_node, IteratorType::instance());
	auto expandable = new ExpandableValue(false, IteratorType::instance(), iteratorEntryPoint, continuation);

	auto tuple = new TupleValue({ expandable, value });

	return
	{
		{ iteratorEntryPoint, tuple },
		{ iteratorEntryPoint, value },
	};
}
