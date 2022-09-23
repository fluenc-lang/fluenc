#include <llvm/IR/Verifier.h>

#include <unordered_map>

#include "Analyzer.h"
#include "FunctionNotFoundException.h"
#include "ValueHelper.h"
#include "ITypeName.h"
#include "UndeclaredIdentifierException.h"
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
#include "nodes/DistributorNode.h"

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

std::vector<DzResult> Analyzer::visit(const BooleanBinaryNode *node, DefaultVisitorContext context) const
{
	UNUSED(node);

	return {{ context.entryPoint, context.values.discard() }};
}

std::vector<DzResult> Analyzer::visit(const FloatBinaryNode *node, DefaultVisitorContext context) const
{
	auto left = context.values.pop();
	auto right = context.values.pop();

	UNUSED(right);

	auto valueFactory = [&]() -> const BaseValue *
	{
		if (node->op == "+")
		{
			return left;
		}

		if (node->op == "-")
		{
			return left;
		}

		if (node->op == "*")
		{
			return left;
		}

		if (node->op == "/")
		{
			return left;
		}

		if (node->op == "<")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->op == "<=")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->op == ">")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->op == ">=")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->op == "==")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->op == "!=")
		{
			return new DummyValue(BooleanType::instance());
		}

		throw new std::exception();
	};

	auto value = valueFactory();

	context.values.push(value);

	return {{ context.entryPoint, context.values }};
}

std::vector<DzResult> Analyzer::visit(const IntegerBinaryNode *node, DefaultVisitorContext context) const
{
	auto left = context.values.pop();
	auto right = context.values.pop();

	UNUSED(right);

	auto valueFactory = [&]() -> const BaseValue *
	{
		if (node->op == "+")
		{
			return left;
		}

		if (node->op == "-")
		{
			return left;
		}

		if (node->op == "*")
		{
			return left;
		}

		if (node->op == "/")
		{
			return left;
		}

		if (node->op == "<")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->op == "<=")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->op == ">")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->op == ">=")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->op == "==")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->op == "!=")
		{
			return new DummyValue(BooleanType::instance());
		}

		if (node->op == "%")
		{
			return left;
		}

		if (node->op == "|")
		{
			return left;
		}

		if (node->op == "&")
		{
			return left;
		}

		if (node->op == "^")
		{
			return left;
		}

		throw new std::exception();
	};

	auto value = valueFactory();

	context.values.push(value);

	return {{ context.entryPoint, context.values }};
}

std::vector<DzResult> Analyzer::visit(const BinaryNode *node, DefaultVisitorContext context) const
{
	auto left = context.values.peek();

	auto leftType = left->type();
	auto operators = leftType->operators();

	auto binary = operators->forBinary(node);

	std::vector<DzResult> results;

	for (auto &[resultEntryPoint, resultValues] : binary->accept(*this, context))
	{
		for (auto &result : node->consumer->accept(*this, { resultEntryPoint, resultValues }))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Analyzer::visit(const ExportedFunctionNode *node, DefaultVisitorContext context) const
{
	UNUSED(node);
	UNUSED(context);

	return std::vector<DzResult>();
}

std::vector<DzResult> Analyzer::visit(const ArrayContinuationNode *node, DefaultVisitorContext context) const
{
	auto value = new ExpandedValue(true
		, node->m_iteratorType
		, context.entryPoint
		, node->m_node
		, node
		, std::vector<const ExpandedValue *>()
		);

	return {{ context.entryPoint, value }};
}

std::vector<DzResult> Analyzer::visit(const ArrayElementNode *node, DefaultVisitorContext context) const
{
	auto value = context.values.require<IndexedValue>(nullptr);

	auto ep = context.entryPoint
		.withIndex(value->index());

	if (node->m_next)
	{
		auto continuation = new ArrayContinuationNode(nullptr, node->m_node, IteratorType::instance());
		auto expandable = new ExpandableValue(true, node->m_arrayType, context.entryPoint, continuation);

		auto tuple = new TupleValue({ expandable, value->subject() });

		auto resultsIfFalse = node->m_next->accept(*this, { ep, context.values });

		std::vector<DzResult> result = {{ ep, tuple }};

		result.insert(end(result), begin(resultsIfFalse), end(resultsIfFalse));

		return result;
	}

	context.values.push(value->subject());

	return {{ ep, context.values }};
}

std::vector<DzResult> Analyzer::visit(const IntegralLiteralNode *node, DefaultVisitorContext context) const
{
	auto llvmContext = context.entryPoint.context();

	auto type = node->m_type->resolve(context.entryPoint);
	auto storageType = type->storageType(*llvmContext);

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

	context.values.push(value);

	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Analyzer::visit(const FloatLiteralNode *node, DefaultVisitorContext context) const
{
	auto llvmContext = context.entryPoint.context();

	auto type = node->m_type->resolve(context.entryPoint);
	auto storageType = type->storageType(*llvmContext);

	auto value = new ScalarValue(type
		, llvm::ConstantFP::get(storageType, node->m_value)
		);

	context.values.push(value);

	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Analyzer::visit(const BooleanLiteralNode *node, DefaultVisitorContext context) const
{
	auto llvmContext = context.entryPoint.context();

	auto valueProvider = [&]
	{
		auto type = BooleanType::instance();
		auto storageType = type->storageType(*llvmContext);

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

	context.values.push(value);

	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Analyzer::visit(const StringLiteralNode *node, DefaultVisitorContext context) const
{
	auto stringType = StringType::get(node->m_value.size());

	auto string = new DummyValue(stringType);

	context.values.push(string);

	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Analyzer::visit(const CharacterLiteralNode *node, DefaultVisitorContext context) const
{
	auto type = Int32Type::instance();

	auto value = new DummyValue(type);

	context.values.push(value);

	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Analyzer::visit(const NothingNode *node, DefaultVisitorContext context) const
{
	context.values.push(WithoutValue::instance());

	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Analyzer::visit(const MemberAccessNode *node, DefaultVisitorContext context) const
{
	auto &locals = context.entryPoint.locals();
	auto &functions = context.entryPoint.functions();
	auto &globals = context.entryPoint.globals();

	for (auto &name : node->m_names)
	{
		auto localsIterator = locals.find(name);

		if (localsIterator != locals.end())
		{
			auto forwarded = localsIterator->second->forward(node->id());

			context.values.push(forwarded);

			return node->m_consumer->accept(*this, context);
		}

		auto functionsIterator = functions.find(name);

		if (functionsIterator != functions.end())
		{
			auto value = new FunctionValue(functionsIterator->second, context.entryPoint);

			context.values.push(value);

			return node->m_consumer->accept(*this, context);
		}

		auto globalsIterator = globals.find(name);

		if (globalsIterator != globals.end())
		{
			std::vector<DzResult> results;

			for (auto &[resultEntryPoint, resultValues] : globalsIterator->second->accept(*this, context))
			{
				for (auto &result : node->m_consumer->accept(*this, { resultEntryPoint, resultValues }))
				{
					results.push_back(result);
				}
			}

			return results;
		}
	}

	throw new UndeclaredIdentifierException(node->m_ast, node->m_names[0]);
}

std::vector<DzResult> Analyzer::visit(const ReferenceSinkNode *node, DefaultVisitorContext context) const
{
	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Analyzer::visit(const LazyEvaluationNode *node, DefaultVisitorContext context) const
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

				for (auto &[resultEntryPoint, resultValues] : iteratable->accept(*this, { entryPoint, Stack() }))
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

						for (auto &[resultEntryPoint, resultValues] : node->accept(*this, { entryPoint, values }))
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

	auto ep = context.entryPoint
		.withIteratorStorage(new DummyIteratorStorage());

	std::vector<DzResult> results;

	for (auto &[resultEntryPoint, resultValues] : digestDepth(ep, context.values, digestDepth))
	{
		for (auto &result : node->m_consumer->accept(*this, { resultEntryPoint, resultValues }))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Analyzer::visit(const FunctionCallNode *node, DefaultVisitorContext context) const
{
	auto findFunction = [&](const std::vector<const Type *> &types) -> const CallableNode *
	{
		auto &functions = context.entryPoint.functions();
		auto &locals = context.entryPoint.locals();

		for (auto &name : node->m_names)
		{
			auto local = locals.find(name);

			if (local != locals.end())
			{
				auto value = dynamic_cast<const FunctionValue *>(local->second);

				return value->function();
			}

			std::map<int8_t, const CallableNode *> candidates;

			for (auto [i, end] = functions.equal_range(name); i != end; i++)
			{
				auto function = i->second;

				auto score = function->signatureCompatibility(context.entryPoint, types);

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

	for (auto &[resultEntryPoint, resultValues] : node->m_evaluation->accept(*this, context))
	{
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
			return function->accept(*this, { resultEntryPoint, resultValues });
		}

		auto functionResults = function->accept(*this, { resultEntryPoint, resultValues });

		for (const auto &[lastEntryPoint, returnValue] : functionResults)
		{
			auto consumerEntryPoint = resultEntryPoint
				.withDepth(lastEntryPoint.depth());

			result.push_back({ consumerEntryPoint, returnValue });
		}
	}

	return result;
}

std::vector<DzResult> Analyzer::visit(const StackSegmentNode *node, DefaultVisitorContext context) const
{
	std::vector<DzResult> result;
	std::vector<DzResult> input = {{ context.entryPoint, Stack() }};

	auto subjectResults = std::accumulate(rbegin(node->m_values), rend(node->m_values), input, [&](auto accumulator, auto argument)
	{
		std::vector<DzResult> results;

		for (auto &[accumulatorEntryPoint, accumulatorValues] : accumulator)
		{
			auto result = argument->accept(*this, { accumulatorEntryPoint, Stack() });

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
		auto callResults = node->m_call->accept(*this, { subjectEntryPoint, subjectValues });

		for (auto &[callEntryPoint, callValues] : callResults)
		{
			auto forwardedValues = context.values;

			for (auto &value : callValues)
			{
				forwardedValues.push(value);
			}

			auto consumerResults = node->m_consumer->accept(*this, { callEntryPoint, forwardedValues });

			for (auto &consumerResult : consumerResults)
			{
				result.push_back(consumerResult);
			}
		}
	}

	return result;
}

std::vector<DzResult> Analyzer::visit(const FunctionCallProxyNode *node, DefaultVisitorContext context) const
{
	auto &functions = context.entryPoint.functions();

	for (auto &name : node->m_names)
	{
		for (auto [i, end] = functions.equal_range(name); i != end; i++)
		{
			auto function = i->second;

			// Naive. Really naive.
			if (function->attribute() == FunctionAttribute::Iterator)
			{
				auto generator = new IteratorValueGenerator(new IteratorType(), node->m_regularCall, context.entryPoint);
				auto lazy = new LazyValue(generator, context.entryPoint);

				auto forwardedValues = context.values;

				forwardedValues.push(lazy);

				std::vector<DzResult> results;

				for (auto &result : node->m_consumer->accept(*this, { context.entryPoint, forwardedValues }))
				{
					results.push_back(result);
				}

				return results;
			}
		}
	}

	std::vector<DzResult> results;

	for (auto &[resultEntryPoint, resultValues] : node->m_regularCall->accept(*this, context))
	{
		for (auto &result : node->m_consumer->accept(*this, { resultEntryPoint, resultValues }))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Analyzer::visit(const JunctionNode *node, DefaultVisitorContext context) const
{
	UNUSED(node);
	UNUSED(context);

	return std::vector<DzResult>();
}

std::vector<DzResult> Analyzer::visit(const InstantiationNode *node, DefaultVisitorContext context) const
{
	auto module = context.entryPoint.module();

	auto dataLayout = module->getDataLayout();

	std::unordered_map<std::string, const BaseValue *> valuesByName;

	std::transform(begin(node->m_fields), end(node->m_fields), std::inserter(valuesByName, begin(valuesByName)), [&](auto field)
	{
		return std::make_pair(field, context.values.pop());
	});

	auto prototype = node->m_prototypeProvider->provide(context.entryPoint, context.values);

	auto prototypeFields = prototype->fields(context.entryPoint, *this);

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

	context.values.push(userTypeValue);

	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Analyzer::visit(const ConditionalNode *node, DefaultVisitorContext context) const
{
	auto condition = context.values.pop();

	UNUSED(condition);

	auto resultsIfTrue = node->m_ifTrue->accept(*this, context);
	auto resultsIfFalse = node->m_ifFalse->accept(*this, context);

	std::vector<DzResult> immediateResults;

	immediateResults.insert(end(immediateResults), begin(resultsIfTrue), end(resultsIfTrue));
	immediateResults.insert(end(immediateResults), begin(resultsIfFalse), end(resultsIfFalse));

	return immediateResults;
}

std::vector<DzResult> Analyzer::visit(const BlockInstructionNode *node, DefaultVisitorContext context) const
{
	return node->m_subject->accept(*this, context);
}

std::vector<DzResult> Analyzer::visit(const EmptyArrayNode *node, DefaultVisitorContext context) const
{
	context.values.push(WithoutValue::instance());

	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Analyzer::visit(const IndexSinkNode *node, DefaultVisitorContext context) const
{
	auto value = context.values.pop();

	auto indexed = new IndexedValue(node->m_index, value);

	context.values.push(indexed);

	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Analyzer::visit(const ArraySinkNode *node, DefaultVisitorContext context) const
{
	auto arrayContents = node->m_firstValue->accept(*this, { context.entryPoint, Stack() });

	auto generator = new ArrayValueGenerator(arrayContents, node->id(), node->m_size);
	auto lazy = new LazyValue(generator, context.entryPoint);

	context.values.push(lazy);

	std::vector<DzResult> results;

	for (auto &[arrayEntryPoint, _] : arrayContents)
	{
		for (auto &result : node->m_consumer->accept(*this, { arrayEntryPoint, context.values }))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Analyzer::visit(const ExpansionNode *node, DefaultVisitorContext context) const
{
	auto expandable = context.values.require<ExpandableValue>(node->m_ast);

	auto continuation = expandable->chain();
	auto provider = expandable->provider();

	auto result = continuation->accept(*this, { *provider, Stack() });

	for (auto &[targetEntryPoint, continuationValues] : result)
	{
		auto value = continuationValues
			.require<ExpandedValue>(nullptr);

		auto tuple = new TupleValue({ value, PlaceholderValue::instance() });

		context.values.push(tuple);

		return node->m_consumer->accept(*this, context);
	}

	throw new std::exception();
}

std::vector<DzResult> Analyzer::visit(const LocalNode *node, DefaultVisitorContext context) const
{
	auto locals = context.entryPoint.locals();

	auto value = context.values.pop();

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

	auto ep = context.entryPoint
		.withLocals(locals);

	return node->m_consumer->accept(*this, { ep, context.values });
}

std::vector<DzResult> Analyzer::visit(const ContinuationNode *node, DefaultVisitorContext context) const
{
	auto next = ValueHelper::extractValues<ExpandedValue>(context.values);

	auto isArray = accumulate(begin(next), end(next), next.size() > 0, [](auto accumulated, auto value)
	{
		return accumulated && value->isArray();
	});

	auto value = new ExpandedValue(isArray
		, node->m_iteratorType
		, context.entryPoint
		, node->m_node
		, node
		, next
		);

	return {{ context.entryPoint, value }};
}

std::vector<DzResult> Analyzer::visit(const UnaryNode *node, DefaultVisitorContext context) const
{
	auto operand = context.values.pop();

	auto resolveOp = [&]
	{
		if (node->m_op == "@")
		{
			return new ForwardedValue(operand);
		}

		throw new std::exception(); // TODO
	};

	auto value = resolveOp();

	context.values.push(value);

	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Analyzer::visit(const TailFunctionCallNode *node, DefaultVisitorContext context) const
{
	auto [score, tailCallTarget, targetValues] = FunctionHelper::tryCreateTailCall(context.entryPoint, context.values, begin(node->m_names), end(node->m_names));

	if (score == 0)
	{
		return std::vector<DzResult>();
	}

	if (score == 1)
	{
		throw new std::exception(); // TODO
	}

	return node->m_regularCall->accept(*this, context);
}

std::vector<DzResult> Analyzer::visit(const FunctionNode *node, DefaultVisitorContext context) const
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

	auto pep = context.entryPoint
		.withValues(context.values);

	std::map<std::string, const BaseValue *> locals;

	for (const auto &argument : node->m_arguments)
	{
		for (auto &[name, value] : handleArgument(argument, context.values.pop(), handleArgument))
		{
			locals[name] = value;
		}
	}

	auto ep = pep
		.withName(node->m_name)
		.markEntry()
		.withLocals(locals)
		.withIteratorStorage(nullptr);

	return node->m_block->accept(*this, { ep, context.values });
}

std::vector<DzResult> Analyzer::visit(const ExportedFunctionTerminatorNode *node, DefaultVisitorContext context) const
{
	UNUSED(node);
	UNUSED(context);

	return std::vector<DzResult>();
}

std::vector<DzResult> Analyzer::visit(const ImportedFunctionNode *node, DefaultVisitorContext context) const
{
	auto returnType = node->m_returnType->resolve(context.entryPoint);

	if (returnType != VoidType::instance())
	{
		return {{ context.entryPoint, new DummyValue(returnType) }};
	}

	return {{ context.entryPoint, Stack() }};
}

std::vector<DzResult> Analyzer::visit(const GlobalNode *node, DefaultVisitorContext context) const
{
	return node->m_value->accept(*this, context);
}

std::vector<DzResult> Analyzer::visit(const ReturnNode *node, DefaultVisitorContext context) const
{
	auto value = context.values.pop();

	if (node->m_chained)
	{
		auto expandable = new ExpandableValue(false, node->m_iteratorType, context.entryPoint, node->m_chained);
		auto tuple = new TupleValue({ expandable, value });

		context.values.push(tuple);

		return node->m_consumer->accept(*this, context);
	}

	context.values.push(value);

	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Analyzer::visit(const ParentInjectorNode *node, DefaultVisitorContext context) const
{
	return node->m_subject->accept(*this, context);
}

std::vector<DzResult> Analyzer::visit(const BlockStackFrameNode *node, DefaultVisitorContext context) const
{
	return node->m_consumer->accept(*this, { context.entryPoint, Stack() });
}

std::vector<DzResult> Analyzer::visit(const TerminatorNode *node, DefaultVisitorContext context) const
{
	UNUSED(node);

	return {{ context.entryPoint, context.values }};
}

std::vector<DzResult> Analyzer::visit(const IteratableNode *node, DefaultVisitorContext context) const
{
	return node->m_iteratable->accept(*this, context);
}

std::vector<DzResult> Analyzer::visit(const ArrayValue *node, DefaultVisitorContext context) const
{
	std::vector<DzResult> results;

	for (auto [_, elementValues] : node->m_values)
	{
		auto iteratorEntryPoint = context.entryPoint
			.withName("__array")
			.markEntry()
			;

		for (auto &result : node->m_iterator->accept(*this, { iteratorEntryPoint, elementValues }))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Analyzer::visit(const IteratorValue *node, DefaultVisitorContext context) const
{
	auto locals = context.entryPoint.locals();

	for (auto &[key, value] : node->m_entryPoint->locals())
	{
		locals[key] = value;
	}

	auto ep = (*node->m_entryPoint)
		.withLocals(locals)
		.withIteratorStorage(context.entryPoint.iteratorStorage());

	return node->m_subject->accept(*this, { ep, Stack() });
}

std::vector<DzResult> Analyzer::visit(const StringIteratable *node, DefaultVisitorContext context) const
{
	auto iteratorEntryPoint = context.entryPoint
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

std::vector<DzResult> Analyzer::visit(const DistributorNode *node, DefaultVisitorContext context) const
{
	std::vector<DzResult> results;

	for (auto &[entryPoint, values] : node->m_subject->accept(*this, context))
	{
		for (auto &result : node->m_consumer->accept(*this, { entryPoint, values }))
		{
			results.push_back(result);
		}
	}

	return results;
}
