#include <llvm/IR/Verifier.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>
#include <llvm/Passes/PassBuilder.h>

#include <range/v3/view.hpp>
#include <fmt/format.h>

#include <unordered_map>
#include <unordered_set>
#include <iostream>

#include "Emitter.h"
#include "ValueHelper.h"
#include "IRBuilderEx.h"
#include "FunctionHelper.h"
#include "indexed.hpp"
#include "InteropHelper.h"
#include "TypeCompatibilityCalculator.h"
#include "ast.hpp"
#include "utility.hpp"

#include "iterators/index_iterator.hpp"

#include "fragments/RenderedNode.h"
#include "fragments/stack_segment.hpp"

#include "types/IOperatorSet.h"
#include "types/Int64Type.h"
#include "types/BooleanType.h"
#include "types/StringType.h"
#include "types/Int32Type.h"
#include "types/IteratorType.h"
#include "types/ExpandedType.h"
#include "types/ByteType.h"
#include "types/ArrayType.h"
#include "types/UserType.h"
#include "types/FunctionType.h"
#include "types/BufferType.h"
#include "types/AggregateType.h"
#include "types/PrototypeField.h"
#include "types/IPrototype.h"
#include "types/structure_type.hpp"

#include "values/AggregateIteratorValueGenerator.h"
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
#include "values/IteratorValueGenerator.h"
#include "values/ArrayValueGenerator.h"
#include "values/PlaceholderValue.h"
#include "values/StringIteratable.h"
#include "values/IteratorValueGeneratorProxy.h"
#include "values/Iterator.h"
#include "values/ExpandedLazyValue.h"
#include "values/BufferValue.h"

#include "exceptions/InvalidFunctionPointerTypeException.h"
#include "exceptions/MissingTailCallException.h"
#include "exceptions/AmbiguousFunctionException.h"
#include "exceptions/MissingDefaultValueException.h"
#include "exceptions/MissingFieldException.h"
#include "exceptions/InvalidArgumentTypeException.h"
#include "exceptions/InvalidOperatorException.h"
#include "exceptions/BinaryTypeMismatchException.h"
#include "exceptions/FunctionNotFoundException.h"
#include "exceptions/UndeclaredIdentifierException.h"

#include "fragments/ArrayValue.h"
#include "fragments/IteratorValue.h"
#include "fragments/IteratableNode.h"
#include "fragments/Pod.h"
#include "fragments/ArrayContinuationFragment.h"
#include "fragments/binary_fragment.hpp"
#include "fragments/unary_fragment.hpp"
#include "fragments/ArrayElementNode.h"
#include "fragments/StaticNode.h"
#include "fragments/junction.hpp"
#include "fragments/pre_evaluation.hpp"
#include "fragments/post_evaluation.hpp"
#include "fragments/RenderedNode.h"
#include "fragments/AllocatorNode.h"

struct johej : public fragment
{
	std::shared_ptr<peg::Ast> ast;
	std::vector<std::string> names;

		// fragment interface
	public:
		std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override
		{
			return visitor.make_function_call(ast, names, context);
		}
};

struct Foo
{
	std::function<std::vector<DzResult>(const Emitter&, DefaultVisitorContext)> func;
};

int8_t signatureCompatibility(const fluenc::function_node* callable, const EntryPoint &entryPoint, const std::vector<const Type *> &values)
{
	auto arguments = callable->signature.arguments;

	if (arguments.size() != values.size())
	{
		return -1;
	}

	std::vector<std::pair<const Type *, const Type *>> types;

	std::transform(begin(arguments), end(arguments), begin(values), std::back_inserter(types), [=](const fluenc::argument_t& argument, auto valueType)
	{
		auto argumentType = arg_type(argument, entryPoint);

		return std::make_pair(argumentType, valueType);
	});

	return std::accumulate(begin(types), end(types), 0, [=](auto score, auto pair) -> int8_t
	{
		if (score < 0)
		{
			return score;
		}

		auto [argumentType, valueType] = pair;

		auto compatibility = TypeCompatibilityCalculator::calculate(entryPoint, valueType, argumentType);

		if (compatibility < 0)
		{
			return compatibility;
		}

		return score + compatibility;
	});
}

std::vector<DzResult> Emitter::visit(const boolean_binary_fragment*node, DefaultVisitorContext context) const
{
	auto left = ValueHelper::getScalar(node->binary->ast, context.entryPoint, context.values);
	auto right = ValueHelper::getScalar(node->binary->ast, context.entryPoint, context.values);

	IRBuilderEx builder(context.entryPoint);

	auto valueFactory = [&]
	{
		if (node->binary->op == "==")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_EQ, left, right);
		}

		if (node->binary->op == "!=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_NE, left, right);
		}

		if (node->binary->op == "&&")
		{
			return builder.createLogicalAnd(left, right);
		}

		if (node->binary->op == "||")
		{
			return builder.createLogicalOr(left, right);
		}

		auto operandType = left->type();
		auto operandTypeName = operandType->name();

		throw InvalidOperatorException(node->binary->ast, node->binary->op, operandTypeName);
	};

	auto value = valueFactory();

	context.values.push(value);

	return {{ context.entryPoint, context.values }};
}

std::vector<DzResult> Emitter::visit(const float_binary_fragment* node, DefaultVisitorContext context) const
{
	auto left = ValueHelper::getScalar(node->binary->ast, context.entryPoint, context.values);
	auto right = ValueHelper::getScalar(node->binary->ast, context.entryPoint, context.values);

	IRBuilderEx builder(context.entryPoint);

	auto valueFactory = [&]
	{
		if (node->binary->op == "+")
		{
			return builder.createFAdd(left, right);
		}

		if (node->binary->op == "-")
		{
			return builder.createFSub(left, right);
		}

		if (node->binary->op == "*")
		{
			return builder.createFMul(left, right);
		}

		if (node->binary->op == "/")
		{
			return builder.createFDiv(left, right);
		}

		if (node->binary->op == "<")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::FCMP_ULT, left, right);
		}

		if (node->binary->op == "<=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::FCMP_ULE, left, right);
		}

		if (node->binary->op == ">")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::FCMP_UGT, left, right);
		}

		if (node->binary->op == ">=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::FCMP_UGE, left, right);
		}

		if (node->binary->op == "==")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::FCMP_UEQ, left, right);
		}

		if (node->binary->op == "!=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::FCMP_UNE, left, right);
		}

		auto operandType = left->type();
		auto operandTypeName = operandType->name();

		throw InvalidOperatorException(node->binary->ast, node->binary->op, operandTypeName);
	};

	auto value = valueFactory();

	context.values.push(value);

	return {{ context.entryPoint, context.values }};
}

std::vector<DzResult> Emitter::visit(const integer_binary_fragment *node, DefaultVisitorContext context) const
{
	auto left = ValueHelper::getScalar(node->binary->ast, context.entryPoint, context.values);
	auto right = ValueHelper::getScalar(node->binary->ast, context.entryPoint, context.values);

	IRBuilderEx builder(context.entryPoint);

	auto valueFactory = [&]
	{
		if (node->binary->op == "+")
		{
			return builder.createAdd(left, right);
		}

		if (node->binary->op == "-")
		{
			return builder.createSub(left, right);
		}

		if (node->binary->op == "*")
		{
			return builder.createMul(left, right);
		}

		if (node->binary->op == "/")
		{
			return builder.createSDiv(left, right);
		}

		if (node->binary->op == "<")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SLT, left, right);
		}

		if (node->binary->op == "<=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SLE, left, right);
		}

		if (node->binary->op == ">")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SGT, left, right);
		}

		if (node->binary->op == ">=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SGE, left, right);
		}

		if (node->binary->op == "==")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_EQ, left, right);
		}

		if (node->binary->op == "!=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_NE, left, right);
		}

		if (node->binary->op == "%")
		{
			return builder.createSRem(left, right);
		}

		if (node->binary->op == "|")
		{
			return builder.createOr(left, right);
		}

		if (node->binary->op == "&")
		{
			return builder.createAnd(left, right);
		}

		if (node->binary->op == "^")
		{
			return builder.createXor(left, right);
		}

		if (node->binary->op == "<<")
		{
			return builder.createLeftShift(left, right);
		}

		if (node->binary->op == ">>")
		{
			return builder.createRightShift(left, right);
		}

		auto operandType = left->type();
		auto operandTypeName = operandType->name();

		throw InvalidOperatorException(node->binary->ast, node->binary->op, operandTypeName);
	};

	auto value = valueFactory();

	context.values.push(value);

	return {{ context.entryPoint, context.values }};
}

std::vector<DzResult> Emitter::visit(const string_binary_fragment *node, DefaultVisitorContext context) const
{
	auto operand = context.values.pop();

	auto operandType = operand->type();
	auto operandTypeName = operandType->name();

	throw InvalidOperatorException(node->binary->ast, node->binary->op, operandTypeName);
}

std::vector<DzResult> Emitter::visit(const array_binary_fragment *node, DefaultVisitorContext context) const
{
	auto left = context.values.require<lazy_value>(node->binary->ast);
	auto right = context.values.require<lazy_value>(node->binary->ast);

	if (node->binary->op == "|")
	{
		std::vector<const BaseValue *> inputs({ new ExpandedLazyValue(right), new ExpandedLazyValue(left) });

		auto generator = new AggregateIteratorValueGenerator(node->binary, inputs);
		auto lazy = new lazy_value(generator, context.entryPoint);

		context.values.push(lazy);

		return fluenc::accept(node->binary->consumer, *this, { context.entryPoint, context.values });
	}

	auto operandType = left->type();
	auto operandTypeName = operandType->name();

	throw InvalidOperatorException(node->binary->ast, node->binary->op, operandTypeName);
}

std::vector<DzResult> Emitter::visit(const without_binary_fragment *node, DefaultVisitorContext context) const
{
	UNUSED(node);

	context.values.discard();

	return {{ context.entryPoint, context.values }};
}

std::vector<DzResult> Emitter::visit(const user_binary_fragment *node, DefaultVisitorContext context) const
{
	UNUSED(node);

	auto left = context.values.pop();
	auto right = context.values.pop();

	if (node->binary->op == "|")
	{
            auto tuple = new tuple_value({ right, left });

		context.values.push(tuple);

		return {{ context.entryPoint, context.values }};
	}

	auto operandType = left->type();
	auto operandTypeName = operandType->name();

	throw InvalidOperatorException(node->binary->ast, node->binary->op, operandTypeName);
}

std::vector<DzResult> Emitter::visit(const fluenc::binary_node *node, DefaultVisitorContext context) const
{
	auto values = context.values;

	auto left = values.pop();
	auto right = values.pop();

	auto leftType = left->type();
	auto rightType = right->type();

	if (TypeCompatibilityCalculator::calculate(context.entryPoint, leftType, rightType) > 1)
	{
		throw BinaryTypeMismatchException(node->ast
			, leftType->name()
			, rightType->name()
			);
	}

	auto operators = leftType->operators();

	if (!operators)
	{
		auto operandTypeName = leftType->name();

		throw InvalidOperatorException(node->ast, node->op, operandTypeName);
	}

	auto binary = operators->forBinary(node);

	std::vector<DzResult> results;

	for (auto &[resultEntryPoint, resultValues] : binary->build(*this, context))
	{
		for (auto &result : fluenc::accept(node->consumer, *this, { resultEntryPoint, resultValues }))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Emitter::visit(const ArrayContinuationNode *node, DefaultVisitorContext context) const
{
	auto llvmContext = context.entryPoint.context();

        auto indexType = int64_type::instance();
	auto storageType = indexType->storageType(*llvmContext);

        auto indexConstant = new scalar_value(indexType
		, llvm::ConstantInt::get(storageType, 1)
		);

	IRBuilderEx builder(context.entryPoint);

	auto load = builder.createLoad(node->m_index, "index");
	auto add = builder.createAdd(load, indexConstant);

	auto value = new ExpandedValue(true
		, node->m_iteratorType
		, context.entryPoint
		, node->m_node
		// , node
		, std::vector<const ExpandedValue *>()
		, add
		);

	return {{ context.entryPoint, value }};
}

std::vector<DzResult> Emitter::visit(const ArrayElementNode *node, DefaultVisitorContext context) const
{
	Stack valuesIfTrue;

	auto llvmContext = context.entryPoint.context();
	auto module = context.entryPoint.module();

	auto dataLayout = module->getDataLayout();

        auto index = context.values.require<reference_value>(node->m_ast);
        auto value = context.values.require<indexed_value>(node->m_ast);

	if (node->m_next)
	{
		auto valuesIfFalse = context.values;

		auto indexType = index->type();
		auto storageType = indexType->storageType(*llvmContext);

		auto ifTrue = createBlock(llvmContext);
		auto ifFalse = createBlock(llvmContext);

		IRBuilderEx builder(context.entryPoint);

		auto indexLoad = builder.createLoad(index, "index");

                auto indexConstant = new scalar_value(indexType
			, llvm::ConstantInt::get(storageType, value->index())
			);

		auto comparison =  builder.createCmp(llvm::CmpInst::Predicate::ICMP_EQ, indexLoad, indexConstant);

		builder.createCondBr(comparison , ifTrue, ifFalse);

		auto epIfFalse = context.entryPoint
			.withIndex(value->index())
			.withBlock(ifFalse);

		auto epIfTrue = context.entryPoint
			.withIndex(value->index())
			.withBlock(ifTrue);

		valuesIfFalse.push(index);

		auto continuation = new ArrayContinuationNode(index, node->m_node, iterator_type::instance());
		auto expandable = new ExpandableValue(true, node->m_arrayType, context.entryPoint, continuation, index);

		auto tuple = new tuple_value({ expandable, value->subject() });

		valuesIfTrue.push(tuple);

		auto resultsIfFalse = node->m_next->build(*this, { epIfFalse, valuesIfFalse });

		std::vector<DzResult> result = {{ epIfTrue, valuesIfTrue }};

		result.insert(end(result), begin(resultsIfFalse), end(resultsIfFalse));

		return result;
	}

	auto ep = context.entryPoint
		.withIndex(value->index());

	context.values.push(value->subject());

	return {{ ep, context.values }};
}

std::vector<DzResult> Emitter::visit(const fluenc::integral_literal_node *node, DefaultVisitorContext context) const
{
	auto llvmContext = context.entryPoint.context();

	auto type = type_for(node->type, context.entryPoint);
	auto storageType = type->storageType(*llvmContext);

	auto valueProvider = [&]
	{
		auto hex = node->value.find("0x");

		if (hex != std::string::npos)
		{
			return llvm::ConstantInt::get((llvm::IntegerType *)storageType, node->value.substr(hex + 2), 16);
		}

		return llvm::ConstantInt::get((llvm::IntegerType *)storageType, node->value, 10);
	};

	auto value = new scalar_value(type
		, valueProvider()
		);

	context.values.push(value);

	return fluenc::accept(node->consumer, *this, context);
}

std::vector<DzResult> Emitter::visit(const fluenc::float_literal_node *node, DefaultVisitorContext context) const
{
	auto llvmContext = context.entryPoint.context();

	auto type = type_for(node->type, context.entryPoint);
	auto storageType = type->storageType(*llvmContext);

	auto value = new scalar_value(type
		, llvm::ConstantFP::get(storageType, node->value)
		);

	context.values.push(value);

	return fluenc::accept(node->consumer, *this, context);
}

std::vector<DzResult> Emitter::visit(const fluenc::boolean_literal_node *node, DefaultVisitorContext context) const
{
	auto llvmContext = context.entryPoint.context();

	auto valueProvider = [&]
	{
		auto type = boolean_type::instance();
		auto storageType = type->storageType(*llvmContext);

		if (node->value == "true")
		{
                    return new scalar_value { type, llvm::ConstantInt::get(storageType, 1) };
		}

		if (node->value == "false")
		{
                    return new scalar_value { type, llvm::ConstantInt::get(storageType, 0) };
		}

		throw std::exception(); // TODO
	};

	auto value = valueProvider();

	context.values.push(value);

	return fluenc::accept(node->consumer, *this, context);
}

std::vector<DzResult> Emitter::visit(const fluenc::string_literal_node *node, DefaultVisitorContext context) const
{
	IRBuilderEx builder(context.entryPoint);

	auto llvmContext = context.entryPoint.context();

	auto stringType = string_type::instance();

        auto string = new scalar_value(stringType
		, builder.createGlobalStringPtr(node->value, "string")
		);

        auto lengthType = int64_type::instance();
	auto lengthStorageType = lengthType->storageType(*llvmContext);

	auto alloc = context.entryPoint.alloc(stringType);
	auto lengthStorage = context.entryPoint.alloc(lengthType);

        auto length = new scalar_value(lengthType
		, llvm::ConstantInt::get(lengthStorageType
			, node->value.size()
			)
		);

	builder.createStore(string, alloc);
	builder.createStore(length, lengthStorage);

	context.values.push(new StringValue(alloc, lengthStorage));

	return fluenc::accept(node->consumer, *this, context);
}

std::vector<DzResult> Emitter::visit(const fluenc::character_literal_node *node, DefaultVisitorContext context) const
{
	auto llvmContext = context.entryPoint.context();

        auto type = int32_type::instance();
	auto storageType = type->storageType(*llvmContext);

        auto value = new scalar_value(type
		, llvm::ConstantInt::get((llvm::IntegerType *)storageType
			, *node->value.begin()
			)
		);

	context.values.push(value);

	return fluenc::accept(node->consumer, *this, context);
}

std::vector<DzResult> Emitter::visit(const fluenc::nothing_node *node, DefaultVisitorContext context) const
{
    context.values.push(without_value::instance());

	return fluenc::accept(node->consumer, *this, context);
}

std::vector<DzResult> Emitter::visit(const fluenc::member_access_node *node, DefaultVisitorContext context) const
{
	auto &locals = context.entryPoint.locals();
	auto &functions = context.entryPoint.functions();
	auto &globals = context.entryPoint.globals();

	for (auto &name : node->names)
	{
		auto localsIterator = locals.find(name);

		if (localsIterator != locals.end())
		{
                    if (auto referenceValue = value_cast<const reference_value *>(localsIterator->second))
			{
				IRBuilderEx builder(context.entryPoint);

				auto load = builder.createLoad(referenceValue);

				context.values.push(load);
			}
			else if (localsIterator->second)
			{
				context.values.push(localsIterator->second);
			}

			return fluenc::accept(node->consumer, *this, context);
		}

		auto functionsIterator = functions.equal_range(name);

		std::vector<fluenc::function_node*> matchingFunctions;

		std::transform(functionsIterator.first, functionsIterator.second, back_inserter(matchingFunctions), [](auto pair)
		{
			return pair.second;
		});

		if (matchingFunctions.size() > 0)
		{
			auto value = new function_value(matchingFunctions, context.entryPoint);

			context.values.push(value);

			return fluenc::accept(node->consumer, *this, context);
		}

		auto globalsIterator = globals.find(name);

		if (globalsIterator != globals.end())
		{
			std::vector<DzResult> results;

			for (auto &[resultEntryPoint, resultValues] : fluenc::accept(globalsIterator->second, *this, context))
			{
				for (auto &result : fluenc::accept(node->consumer, *this, { resultEntryPoint, resultValues }))
				{
					results.push_back(result);
				}
			}

			return results;
		}
	}

	throw UndeclaredIdentifierException(node->ast, node->names[0]);
}

std::vector<DzResult> Emitter::visit(const fluenc::reference_sink_node *node, DefaultVisitorContext context) const
{
	auto makeReference = [&](const BaseValue *value, auto &recurse) -> const BaseValue *
	{
		IRBuilderEx builder(context.entryPoint);

                if (auto typedValue = value_cast<const scalar_value *>(value))
		{
			auto argumentType = typedValue->type();

			auto alloc = context.entryPoint.alloc(argumentType);

			builder.createStore(typedValue, alloc);

			return alloc;
		}

                if (auto referenceValue = value_cast<const reference_value *>(value))
		{
                    return referenceValue->clone(context.entryPoint, clone_strategy::Value);
		}

                if (auto tupleValue = value_cast<const tuple_value *>(value))
		{
			auto tupleValues = tupleValue->values();

			std::vector<const BaseValue *> values;

			std::transform(tupleValues.begin(), tupleValues.end(), std::back_inserter(values), [&](auto value)
			{
				return recurse(value, recurse);
			});

                        return new tuple_value(values);
		}

                if (auto userTypeValue = value_cast<const user_type_value *>(value))
		{
			auto fields = userTypeValue->fields();

                        std::vector<const named_value *> values;

			std::transform(begin(fields), end(fields), std::back_inserter(values), [&](auto field)
			{
                            return new named_value { field->name(), recurse(field->value(), recurse) };
			});

                        return new user_type_value { userTypeValue->prototype(), values };
		}

		return value;
	};

	auto value = makeReference(context.values.pop(), makeReference);

	context.values.push(value);

	return fluenc::accept(node->consumer, *this, context);
}

std::vector<DzResult> Emitter::visit(const pre_evaluation_node *node, DefaultVisitorContext context) const
{
	auto digestDepth = [&, this](const EntryPoint &entryPoint, Stack values, auto &recurse) -> std::vector<DzResult>
	{
		for (auto i = 0u; i < values.size(); i++)
		{
			auto value = values.pop();

			if (auto lazy = value_cast<const ExpandedLazyValue *>(value))
			{
				auto iteratable = lazy->subject()->generate(entryPoint);

				std::vector<DzResult> results;

				for (auto &[resultEntryPoint, resultValues] : iteratable->build(*this, { entryPoint, Stack() }))
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

	std::vector<DzResult> results;

	for (auto &[resultEntryPoint, resultValues] : digestDepth(context.entryPoint, context.values, digestDepth))
	{
		for (auto &result : fluenc::accept(node->consumer, *this, { resultEntryPoint, resultValues }))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Emitter::visit(const post_evaluation_node *node, DefaultVisitorContext context) const
{
	UNUSED(node);

	auto digestDepth = [&, this](const EntryPoint &entryPoint, Stack values, auto &recurse) -> std::vector<DzResult>
	{
		for (auto i = 0u; i < values.size(); i++)
		{
			auto value = values.pop();

			if (auto iterator = value_cast<const Iterator *>(value))
			{
				std::vector<DzResult> results;

				for (auto &[resultEntryPoint, resultValues] : iterator->generate(*this, { entryPoint, Stack() }))
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

			if (auto forwarded = value_cast<const ForwardedValue *>(value))
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

			if (auto tuple = value_cast<const tuple_value *>(value))
			{
				for (auto &element : tuple->values())
				{
					if (auto expanded = value_cast<const ExpandedValue *>(element))
					{
						std::vector<DzResult> results;

						auto node = expanded->node();

						auto pre = new pre_evaluation_node(fluenc::terminator_node::instance());

						for (auto &[rep, rva] : visit(pre, { entryPoint, expanded->values() }))
						{
							for (auto &[inputEntryPoint, inputValues] : recurse(rep, rva, recurse))
							{
								for (auto &[resultEntryPoint, resultValues] : node->build(*this, { inputEntryPoint, inputValues }))
								{
									auto forwardedEntryPoint = entryPoint
										.withBlock(resultEntryPoint.block());

									for (auto &[finalEntryPoint, finalValues] : recurse(forwardedEntryPoint, resultValues, recurse))
									{
										auto forwardedValues = values;

										for (auto resultValue : finalValues)
										{
											forwardedValues.push(resultValue);
										}

										results.push_back({ finalEntryPoint, forwardedValues });
									}
								}
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
                                    auto digestedTuple = new tuple_value({ digestedValues.begin(), digestedValues.end() });

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

	return digestDepth(context.entryPoint, context.values, digestDepth);
}

std::vector<DzResult> Emitter::make_function_call(const std::shared_ptr<peg::Ast>& ast, const std::vector<std::string>& names, DefaultVisitorContext context) const
{
	auto functionsForName = [&](const std::string &name)
	{
		auto &functions = context.entryPoint.functions();
		auto &locals = context.entryPoint.locals();

		auto local = locals.find(name);

		if (local != locals.end())
		{
			auto value = value_cast<const function_value *>(local->second);

			if (!value)
			{
				throw InvalidFunctionPointerTypeException(ast, name);
			}

			return value->functions();
		}

		auto matchingFunctions = functions.equal_range(name);

		std::vector<fluenc::function_node*> result;

		std::transform(matchingFunctions.first, matchingFunctions.second, back_inserter(result), [](auto pair)
		{
			return pair.second;
		});

		return result;
	};

	auto findFunction = [&](const std::vector<const Type *> &types) -> std::pair<fluenc::function_node*, tried_t>
	{
		tried_t tried;

		for (auto &name : names)
		{
			std::map<int8_t, fluenc::function_node*> candidates;

			for (auto &function : functionsForName(name))
			{
				auto score = signatureCompatibility(function, context.entryPoint, types);

				if (score < 0)
				{
					tried.push_back({ score, function });

					continue;
				}

				auto candidate = candidates.find(score);

				if (candidate != candidates.end())
				{
					std::vector<fluenc::function_node*> functions = { candidate->second, function };

					throw AmbiguousFunctionException(ast
						, functions
						, context.entryPoint
						);
				}

				candidates[score] = function;
			}

			if (candidates.size() > 0)
			{
				auto [score, function] = *candidates.begin();

				return { function, tried };
			}
		}

		return { nullptr, tried };
	};

	auto [score, _1, _2] = FunctionHelper::tryCreateTailCall(context.entryPoint, context.values, begin(names), end(names));

	if (score == 0)
	{
		throw MissingTailCallException(ast);
	}

	Emitter nested;
	nested.function_ = names;

	auto llvmContext = context.entryPoint.context();

	std::vector<DzResult> result;

	auto evaluation = new post_evaluation_node();

	for (auto &[resultEntryPoint, resultValues] : nested.visit(evaluation, context))
	{
		auto block = resultEntryPoint.block();

		std::vector<const Type *> types;

		std::transform(resultValues.rbegin(), resultValues.rend(), std::back_inserter(types), [](auto value)
		{
			return value->type();
		});

		auto [function, tried] = findFunction(types);

		if (!function)
		{
			for (auto &[score, callable] : tried)
			{
				std::cout << static_cast<int32_t>(score) << " " << callable->signature.name << "(";

				auto arguments = callable->signature.arguments;

				for (auto i = begin(arguments); i != end(arguments); i++)
				{
					if (i != begin(arguments))
					{
						std::cout << ", ";
					}

					auto argumentType = arg_type(*i, context.entryPoint);

					std::cout << argumentType->name();
				}

				std::cout << ")" << std::endl;
			}

			throw FunctionNotFoundException(ast, names[0], types, tried);
		}

		auto functionBlock = createBlock(llvmContext);

		linkBlocks(block, functionBlock);

		auto functionEntryPoint = resultEntryPoint
			.withBlock(functionBlock);

		if (function->type == fluenc::function_type::imported)
		{
			return nested.visit(function, { functionEntryPoint, resultValues });
		}

		auto functionResults = nested.visit(function, { functionEntryPoint, resultValues });

		for (const auto &[lastEntryPoint, returnValue] : functionResults)
		{
			auto consumerBlock = createBlock(llvmContext);

			linkBlocks(lastEntryPoint.block(), consumerBlock);

			auto consumerEntryPoint = functionEntryPoint
				.withDepth(lastEntryPoint.depth())
				.withBlock(consumerBlock);

			result.push_back({ consumerEntryPoint, returnValue });
		}
	}

	return result;
}

// std::vector<DzResult> Emitter::visit(const fluenc::function_call_node *node, DefaultVisitorContext context) const
// {
// 	return make_function_call(node->ast, node->names, context);
// }

// int order(const fluenc::function_call_node* node, const EntryPoint& entryPoint)
// {
// 	auto &functions = entryPoint.functions();

// 	for (auto &name : node->names)
// 	{
// 		auto iterator = functions.find(name);

// 		if (iterator == functions.end())
// 		{
// 			return -1;
// 		}

// 		if (attribute(iterator->second) == fluenc::function_attribute::iterator)
// 		{
// 			return 1;
// 		}
// 	}

// 	return -1;
// }

std::vector<DzResult> Emitter::visit(const stack_segment_node *node, DefaultVisitorContext context) const
{
	std::vector<DzResult> result;
	std::vector<DzResult> input = {{ context.entryPoint.withIndex(-1), Stack() }};

	std::vector<fluenc::indexed<fluenc::expression_t>> orderedValues;

	std::span<const BaseValue* const> v(context.values.begin(), node->values.size());

	std::transform(begin(node->values), end(node->values), fluenc::index_iterator(), std::back_inserter(orderedValues), [](auto value, auto index) -> fluenc::indexed<fluenc::expression_t>
	{
		return { index, value };
	});

	auto sink = new fluenc::reference_sink_node(fluenc::terminator_node::instance());
	auto preEvaluation = new pre_evaluation_node(sink);

	auto subjectResults = std::accumulate(begin(orderedValues), end(orderedValues), input, [&](auto accumulator, auto argument)
	{
		std::vector<DzResult> results;

		for (auto &[accumulatorEntryPoint, accumulatorValues] : accumulator)
		{
			auto result = accept(argument.value, *this, { accumulatorEntryPoint, Stack() });

			for (auto &[resultEntryPoint, resultValues] : result)
			{
				for (auto &[rep, rva] : visit(preEvaluation, { resultEntryPoint, resultValues }))
				{
					auto scopedReturnValues = accumulatorValues;

					for (auto resultValue : rva)
					{
						auto returnValue = new indexed_value{ argument.index, resultValue };

						scopedReturnValues.push(returnValue);
					}

					results.push_back({ rep, scopedReturnValues });
				}
			}
		}

		return results;
	});

	for (auto &[subjectEntryPoint, subjectValues] : subjectResults)
	{
		std::multimap<int, const BaseValue *, std::greater<int>> indexedValues;

		for (auto &value : subjectValues)
		{
			auto indexed = static_cast<const indexed_value *>(value);

			indexedValues.insert({ indexed->index(), indexed->subject() });
		}

		Stack pointersToValues;

		for (auto [_, value] : indexedValues)
		{
			pointersToValues.push(value);
		}

		auto llvmContext = subjectEntryPoint.context();

		auto preliminaryBlock = createBlock(llvmContext);

		auto preliminaryEntryPoint = subjectEntryPoint
			.withBlock(preliminaryBlock)
			;

		auto pod = new Pod(node->call, context.values);
		auto callResults = pod->build(*this, { preliminaryEntryPoint, pointersToValues });

		auto producesIterator = std::any_of(begin(callResults), end(callResults), [](auto pair)
		{
			auto &[_, callValues] = pair;

			if (callValues.size() <= 0)
			{
				return false;
			}

			auto returnValue = callValues.peek();

			return returnValue->id() == value_id::tuple;
		});

		if (producesIterator)
		{
			auto iterator = new Iterator(pod, new iterator_type(), preliminaryEntryPoint, pointersToValues, callResults);

			result.push_back({ subjectEntryPoint, iterator });
		}
		else
		{
			linkBlocks(subjectEntryPoint.block(), preliminaryBlock);

			std::vector<DzResult> preliminaryResults;

			for (auto &[callEntryPoint, callValues] : callResults)
			{
				auto consumerResults = fluenc::accept(node->consumer, *this, { callEntryPoint, callValues });

				for (auto &consumerResult : consumerResults)
				{
					result.push_back(consumerResult);
				}
			}
		}
	}

	return result;
}

std::vector<DzResult> Emitter::visit(const fluenc::function_call_node *node, DefaultVisitorContext context) const
{
	auto llvmContext = context.entryPoint.context();

	std::vector<DzResult> results;

	auto preliminaryBlock = createBlock(llvmContext);

	auto preliminaryEntryPoint = new EntryPoint(context.entryPoint
		.withBlock(preliminaryBlock)
		);

	auto asdad = new johej();
	asdad->ast = node->ast;
	asdad->names = node->names;

	auto segment = new stack_segment_node(node->values, asdad, fluenc::terminator_node::instance());

	auto preliminaryResults = visit(segment, { preliminaryEntryPoint->detach(), context.values });

	for (auto &[_, preliminaryValues] : preliminaryResults)
	{
		if (preliminaryValues.size() <= 0)
		{
			continue;
		}

		auto returnValue = preliminaryValues.peek();

		if (returnValue->id() == value_id::iterator)
		{
			auto subject = new IteratorValueGenerator(new iterator_type(), segment, context.entryPoint);
			auto generator = new IteratorValueGeneratorProxy(subject, preliminaryEntryPoint, preliminaryResults);
			auto lazy = new lazy_value(generator, context.entryPoint);

			auto forwardedValues = context.values;

			forwardedValues.push(lazy);

			std::vector<DzResult> results;

			for (auto &result : fluenc::accept(node->consumer, *this, { context.entryPoint, forwardedValues }))
			{
				results.push_back(result);
			}

			return results;
		}
	}

	preliminaryEntryPoint->setParent(context.entryPoint);

	linkBlocks(context.entryPoint.block(), preliminaryBlock);

	auto rendered = new RenderedNode {
		.result = preliminaryResults
	};

	auto extension = new fluenc::extension_node(rendered);

	for (auto &[resultEntryPoint, resultValues] : visit(extension, context))
	{
		auto consumerEntryPoint = resultEntryPoint
			.withIndex(context.entryPoint.index());

		for (auto &result : accept(node->consumer, *this, { consumerEntryPoint, resultValues }))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Emitter::visit(const junction_node *node, DefaultVisitorContext context) const
{
	struct type_comparer
	{
		bool operator()(const Type *x, const Type *y) const
		{
			if (x == y)
			{
				return false;
			}

			if (x->id() == type_id::Tuple)
			{
				return true;
			}

			if (y->id() == type_id::Tuple)
			{
				return false;
			}

			return std::less<const Type *>()(x, y);
		}
	};

	struct SingleResult
	{
		const EntryPoint entryPoint;
		const BaseValue *value;
	};

	auto join = [this](const std::vector<SingleResult> &range, const EntryPoint &entryPoint) -> const BaseValue *
	{
		auto [_, first] = *range.begin();

                auto alloc = first->clone(entryPoint, clone_strategy::Storage);

		for (auto &[resultEntryPoint, value] : range)
		{
			auto transferEntryPoint = ValueHelper::transferValue(resultEntryPoint, value, alloc, *this);

			linkBlocks(transferEntryPoint.block(), entryPoint.block());
		}

		return alloc;
	};

	auto tryJoin = [&](const std::vector<SingleResult> &values, const EntryPoint &entryPoint) -> SingleResult
	{
		if (values.size() > 1)
		{
			auto context = entryPoint.context();

			auto junctionBlock = llvm::BasicBlock::Create(*context, "junction");

			auto junctionEntryPoint = entryPoint
				.withBlock(junctionBlock);

			auto joined = join(values, junctionEntryPoint);

			return { junctionEntryPoint, joined };
		}

		return values[0];
	};

	auto inputResults = fluenc::accept(node->subject, *this, context);

	std::vector<DzResult> outputResults;

	std::map<const Type *, std::vector<SingleResult>, type_comparer> groupedResults;

	for (auto &result : inputResults)
	{
		auto [resultEntryPoint, resultValues] = result;

		if (resultValues.size() != 1)
		{
			outputResults.push_back(result);

			continue;
		}

		auto value = resultValues.pop();

		groupedResults[value->type()].push_back({ resultEntryPoint, value });
	}

	for (auto &[type, inputValues] : groupedResults)
	{
		auto [joinedEntryPoint, joinedValue] = tryJoin(inputValues, context.entryPoint);

		outputResults.push_back({ joinedEntryPoint, joinedValue });
	}

	return outputResults;
}

std::vector<DzResult> Emitter::visit(const fluenc::instantiation_node *node, DefaultVisitorContext context) const
{
	std::unordered_map<std::string, const BaseValue *> valuesByName;

	std::transform(begin(node->fields), end(node->fields), std::inserter(valuesByName, begin(valuesByName)), [&](auto field)
	{
		return std::make_pair(field, context.values.pop());
	});

        auto [prototype, pep, prototypeFields] = std::visit([&](auto&& prototype) -> std::tuple<const structure_type*, EntryPoint, std::vector<structure_field_t>> {
		using type = std::decay_t<decltype(prototype)>;

		if constexpr (std::is_same_v<type, fluenc::default_prototype>)
		{
			auto p = static_cast<const structure_type*>(type_for(prototype.type, context.entryPoint));
			auto subject = p->subject();

			auto [k, q] = default_fields(context.entryPoint, *this, subject);

			return { p, k, q };
		}

		if constexpr (std::is_same_v<type, fluenc::with_prototype>)
		{
                    auto addressOfValue = context.values.require<user_type_value>(nullptr);

			auto fields = addressOfValue->fields();

			std::vector<structure_field_t> results;

			std::transform(begin(fields), end(fields), std::back_insert_iterator(results), [](auto field) -> structure_field_t
			{
				return { field->name(), field->value(), field->type() };
			});

			return { addressOfValue->prototype(), context.entryPoint, results };
		}

		throw std::exception(); // TODO
	}
	, node->prototype);

	auto prototypeEntryPoint = pep;

        std::vector<const named_value *> namedValues;

        std::transform(begin(prototypeFields), end(prototypeFields), std::back_inserter(namedValues), [&](auto field) -> const named_value *
	{
		auto valueByName = valuesByName.find(field.name);

		if (valueByName != valuesByName.end())
		{
			auto value = valueByName->second;

			valuesByName.erase(valueByName);

                        return new named_value { field.name, value };
		}

		if (!field.default_value)
		{
			throw MissingDefaultValueException(node->ast, field.name);
		}

                return new named_value { field.name, field.default_value };
	});

	for (auto &[name, _] : valuesByName)
	{
		throw MissingFieldException(node->ast, prototype->name(), name);
	}

	IRBuilderEx builder(prototypeEntryPoint);

        std::vector<const named_value *> finalValues;

        std::transform(begin(namedValues), end(namedValues), std::back_inserter(finalValues), [&](auto field) -> const named_value *
	{
		auto value = field->value();

                if (auto typedValue = value_cast<const scalar_value *>(value))
		{
			auto type = typedValue->type();

			auto alloc = prototypeEntryPoint.alloc(type);

			builder.createStore(typedValue, alloc);

                        return new named_value { field->name(), alloc };
		}

		return field;
	});

        auto userTypeValue = new user_type_value(prototype, finalValues);

	context.values.push(userTypeValue);

	return fluenc::accept(node->consumer, *this, { prototypeEntryPoint, context.values });
}

std::vector<DzResult> Emitter::visit(const fluenc::conditional_node *node, DefaultVisitorContext context) const
{
	struct SingleResult
	{
		const EntryPoint entryPoint;
                const scalar_value *value;
	};

	auto llvmContext = context.entryPoint.context();
	auto module = context.entryPoint.module();

	auto block = context.entryPoint.block();

	auto dataLayout = module->getDataLayout();

	block->setName("condition");

	auto ifTrue = createBlock(llvmContext);
	auto ifFalse = createBlock(llvmContext);

	IRBuilderEx builder(context.entryPoint);

	auto conditional = ValueHelper::getScalar(node->ast, context.entryPoint, context.values);

	builder.createCondBr(conditional, ifTrue, ifFalse);

	auto epIfFalse = context.entryPoint
		.withName("ifFalse")
		.withBlock(ifFalse);

	auto epIfTrue = context.entryPoint
		.withName("ifTrue")
		.withBlock(ifTrue);

	auto resultsIfTrue = fluenc::accept(node->if_true, *this, { epIfTrue, context.values });
	auto resultsIfFalse = fluenc::accept(node->if_false, *this, { epIfFalse, context.values });

	std::vector<DzResult> immediateResults;

	immediateResults.insert(end(immediateResults), begin(resultsIfTrue), end(resultsIfTrue));
	immediateResults.insert(end(immediateResults), begin(resultsIfFalse), end(resultsIfFalse));

	std::multimap<const Type *, SingleResult> groupedResults;

	for (auto &result : immediateResults)
	{
		auto [resultEntryPoint, resultValues] = result;

		if (resultValues.size() != 1)
		{
			continue;
		}

                auto value = resultValues.request<scalar_value>();

		if (value)
		{
			groupedResults.insert({ value->type(), { resultEntryPoint, value } });
		}
		else
		{
			return immediateResults;
		}
	}

	std::vector<DzResult> mergedResults;

	for(auto it = begin(groupedResults)
		; it != end(groupedResults)
		; it = upper_bound(it, end(groupedResults), *it, &compareKey<const Type *, SingleResult>)
		)
	{
		auto [type, _] = *it;

		auto range = groupedResults.equal_range(type);

		auto alloc = context.entryPoint.alloc(type);

		auto mergeBlock = llvm::BasicBlock::Create(*llvmContext, "merge");

		for (auto i = range.first; i != range.second; i++)
		{
			auto [resultEntryPoint, value] = i->second;

			auto resultBlock = resultEntryPoint.block();

			IRBuilderEx resultBuilder(resultEntryPoint);

			resultBuilder.createStore(value, alloc);

			linkBlocks(resultBlock, mergeBlock);
		}

		auto mergeEntryPoint = context.entryPoint
			.withBlock(mergeBlock);

		IRBuilderEx mergeBuilder(mergeEntryPoint);

		auto mergeLoad = mergeBuilder.createLoad(alloc, "mergeLoad");

		auto mergeValues = context.values;

		mergeValues.push(mergeLoad);

		mergedResults.push_back({ mergeEntryPoint, mergeValues });
	}

	return mergedResults;
}

std::vector<DzResult> Emitter::visit(const fluenc::block_instruction_node *node, DefaultVisitorContext context) const
{
	return fluenc::accept(node->subject, *this, { context.entryPoint, Stack() });
}

std::vector<DzResult> Emitter::visit(const fluenc::empty_array_node *node, DefaultVisitorContext context) const
{
    context.values.push(without_value::instance());

	return fluenc::accept(node->consumer, *this, context);
}

std::vector<DzResult> Emitter::visit(const fluenc::index_sink_node *node, DefaultVisitorContext context) const
{
	auto value = context.values.pop();

	auto indexed = new indexed_value(node->index, value);

	context.values.push(indexed);

	return fluenc::accept(node->consumer, *this, context);
}

std::vector<DzResult> Emitter::visit(const fluenc::array_sink_node *node, DefaultVisitorContext context) const
{
	auto arrayContents = fluenc::accept(node->first_value, *this, { context.entryPoint, Stack() });

	auto generator = new ArrayValueGenerator(arrayContents, node->ast, node->size);
	auto lazy = new lazy_value(generator, context.entryPoint);

	context.values.push(lazy);

	std::vector<DzResult> results;

	for (auto &[arrayEntryPoint, _] : arrayContents)
	{
		for (auto &result : fluenc::accept(node->consumer, *this, { arrayEntryPoint, context.values }))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Emitter::visit(const fluenc::expansion_node *node, DefaultVisitorContext context) const
{
	auto block = context.entryPoint.block();

	auto value = context.values.pop();

	if (auto expandable = value_cast<const ExpandableValue *>(value))
	{
		auto continuation = expandable->chain();
		auto provider = expandable->provider();

		auto continuationEntryPoint = (*provider)
			.withBlock(block)
			.withIteratorType(expandable->expandedType());

		auto result = continuation->build(*this, { continuationEntryPoint, Stack() });

		for (auto &[targetEntryPoint, continuationValues] : result)
		{
			auto value = continuationValues
				.require<ExpandedValue>(nullptr);

			auto tuple = new tuple_value({ value, placeholder_value::instance() });

			context.values.push(tuple);

			auto consumerEntryPoint = context.entryPoint
				.withBlock(targetEntryPoint.block());

			return fluenc::accept(node->consumer, *this, { consumerEntryPoint, context.values });
		}

		throw std::exception();
	}
        else if (auto lazy = value_cast<const lazy_value *>(value))
	{
		context.values.push(new ExpandedLazyValue(lazy));

		return fluenc::accept(node->consumer, *this, context);
	}
	else if (auto string = value_cast<const StringValue *>(value))
	{
		context.values.push(new ExpandedLazyValue(string->iterator(context.entryPoint)));

		return fluenc::accept(node->consumer, *this, context);
	}
	else if (auto buffer = value_cast<const BufferValue *>(value))
	{
		context.values.push(new ExpandedLazyValue(buffer->iterator(context.entryPoint)));

		return fluenc::accept(node->consumer, *this, context);
	}
	else
	{
		context.values.push(value);

		return fluenc::accept(node->consumer, *this, context);
	}
}

std::vector<DzResult> Emitter::visit(const fluenc::local_node *node, DefaultVisitorContext context) const
{
	auto locals = context.entryPoint.locals();

	auto value = context.values.pop();

        if (auto userValue = value_cast<const user_type_value * >(value))
	{
		auto fields = userValue->fields();

		std::transform(begin(fields), end(fields), std::inserter(locals, begin(locals)), [=](auto field) -> std::pair<std::string, const BaseValue *>
		{
			std::stringstream ss;
			ss << node->name;
			ss << ".";
			ss << field->name();

			return { ss.str(), field->value() };
		});
	}

	if (auto lazyValue = value_cast<const lazy_value *>(value))
	{
		auto type = lazyValue->type();

		// Optimization: If iteration can be represented as an array, create
		// a new, temporary array and copy the iteration result into it.
		// This will ensure that multiple iterations are cheap.
		//
		if (type->id() == type_id::Array)
		{
			auto allocator = new DefaultNode(type, fluenc::terminator_node::instance());

			auto extension = new fluenc::extension_node(allocator);

			for (auto &result : visit(extension, { context.entryPoint, Stack() }))
			{
				auto array = result.values.require<lazy_value>(nullptr);

				auto assignmentEntryPoint = array->assignFrom(result.entryPoint, lazyValue, *this);

				locals[node->name] = array;

				auto ep = assignmentEntryPoint
					.withLocals(locals);

				return fluenc::accept(node->consumer, *this, { ep, context.values });
			}
		}
	}

	locals[node->name] = value;

	auto ep = context.entryPoint
		.withLocals(locals);

	return fluenc::accept(node->consumer, *this, { ep, context.values });
}

struct F : public fragment
{
	fluenc::expression_t subject;

	std::vector<DzResult> build(const Emitter& visitor, DefaultVisitorContext context) const override
	{
		return fluenc::accept(subject, visitor, context);
	}
};

struct Q : public fragment
{
	fluenc::expression_t subject;
	std::vector<std::string> function;

	std::vector<DzResult> build(const Emitter& visitor, DefaultVisitorContext context) const override
	{
		std::vector<DzResult> results;

		for (auto [entryPoint, values] : fluenc::accept(subject, visitor, context))
		{
			auto inputValues = values;
			auto tailCallValues = entryPoint.values();
			auto iteratorType = entryPoint.iteratorType();

			std::vector<const Type *> types = { iteratorType->iteratorType() };

			std::transform(inputValues.begin(), inputValues.end(), back_inserter(types), [](auto value)
			{
				return value->type();
			});

			std::vector<const BaseValue *> cloned;

			std::transform(inputValues.begin(), inputValues.end(), back_inserter(cloned), [&](auto value)
			{
					return value->clone(entryPoint, clone_strategy::Value);
			});

			auto next = ValueHelper::extractValues<ExpandedValue>(values);

			auto isArray = accumulate(begin(next), end(next), next.size() > 0, [](auto accumulated, auto value)
			{
				return accumulated && value->isArray();
			});

			auto k = new johej();
			k->names = function;

			auto value = new ExpandedValue(isArray
				, expanded_type::get(types)
				, entryPoint
				, k
				// , q
				, next
				, cloned
				);

			results.push_back({ entryPoint, value });
		}

		return results;
	}
};

std::vector<DzResult> Emitter::visit(const boolean_unary_fragment *node, DefaultVisitorContext context) const
{
	auto operand = ValueHelper::getScalar(node->unary->ast, context.entryPoint, context.values);

	IRBuilderEx builder(context.entryPoint);

	auto valueFactory = [&]
	{
		if (node->unary->op == "!")
		{
			return builder.createNot(operand);
		}

		auto operandType = operand->type();
		auto operandTypeName = operandType->name();

		throw InvalidOperatorException(node->unary->ast, node->unary->op, operandTypeName);
	};

	auto value = valueFactory();

	context.values.push(value);

	return {{ context.entryPoint, context.values }};
}

std::vector<DzResult> Emitter::visit(const float_unary_fragment *node, DefaultVisitorContext context) const
{
	auto operand = context.values.pop();

	auto operandType = operand->type();
	auto operandTypeName = operandType->name();

	throw InvalidOperatorException(node->unary->ast, node->unary->op, operandTypeName);
}

std::vector<DzResult> Emitter::visit(const integer_unary_fragment *node, DefaultVisitorContext context) const
{
	auto operand = context.values.pop();

	auto operandType = operand->type();
	auto operandTypeName = operandType->name();

	throw InvalidOperatorException(node->unary->ast, node->unary->op, operandTypeName);
}

std::vector<DzResult> Emitter::visit(const string_unary_fragment *node, DefaultVisitorContext context) const
{
	auto operand = context.values.pop();

	auto valueFactory = [&]
	{
		if (node->unary->op == "@")
		{
			return new ForwardedValue(operand);
		}

		auto operandType = operand->type();
		auto operandTypeName = operandType->name();

		throw InvalidOperatorException(node->unary->ast, node->unary->op, operandTypeName);
	};

	auto value = valueFactory();

	context.values.push(value);

	return fluenc::accept(node->unary->consumer, *this, context);
}

std::vector<DzResult> Emitter::visit(const array_unary_fragment *node, DefaultVisitorContext context) const
{
	auto operand = context.values.pop();

	auto operandType = operand->type();
	auto operandTypeName = operandType->name();

	throw InvalidOperatorException(node->unary->ast, node->unary->op, operandTypeName);
}

std::vector<DzResult> Emitter::visit(const without_unary_fragment *node, DefaultVisitorContext context) const
{
	UNUSED(node);

	return {{ context.entryPoint, context.values }};
}

std::vector<DzResult> Emitter::visit(const user_unary_fragment *node, DefaultVisitorContext context) const
{
	auto operand = context.values.pop();

	auto operandType = operand->type();
	auto operandTypeName = operandType->name();

	throw InvalidOperatorException(node->unary->ast, node->unary->op, operandTypeName);
}

std::vector<DzResult> Emitter::visit(const fluenc::unary_node *node, DefaultVisitorContext context) const
{
	auto operand = context.values.peek();

	auto operandType = operand->type();
	auto operators = operandType->operators();

	if (!operators)
	{
		auto operandTypeName = operandType->name();

		throw InvalidOperatorException(node->ast, node->op, operandTypeName);
	}

	auto unary = operators->forUnary(node);

	std::vector<DzResult> results;

	for (auto &[resultEntryPoint, resultValues] : unary->build(*this, context))
	{
		for (auto &result : fluenc::accept(node->consumer, *this, { resultEntryPoint, resultValues }))
		{
			results.push_back(result);
		}
	}

	return results;
}

struct ksadjasd : public fragment
{
	const fluenc::tail_function_call_node *node;

	std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override
	{
		auto [score, tailCallTarget, targetValues] = FunctionHelper::tryCreateTailCall(context.entryPoint, context.values, begin(node->names), end(node->names));

		if (score == 0)
		{
			std::vector<const BaseValue *> cloned;

			std::transform(context.values.begin(), context.values.end(), back_inserter(cloned), [&](auto value)
			{
				return value->clone(context.entryPoint, clone_strategy::Value);
			});

			auto zipped = ranges::views::zip(cloned, targetValues);

			auto resultEntryPoint = std::accumulate(zipped.begin(), zipped.end(), context.entryPoint, [&](auto accumulatedEntryPoint, auto result)
			{
				auto [value, storage] = result;

				return ValueHelper::transferValue(accumulatedEntryPoint, value, storage, visitor);
			});

			linkBlocks(resultEntryPoint.block(), tailCallTarget->block());

			return std::vector<DzResult>();
		}

		if (score == 1)
		{
			throw std::exception(); // TODO
		}

		return visitor.make_function_call(node->ast, node->names, context);
	}
};

std::vector<DzResult> Emitter::visit(const fluenc::tail_function_call_node *node, DefaultVisitorContext context) const
{
	auto extension = new ksadjasd();
	extension->node = node;

	auto k = new stack_segment_node(node->values, extension, node->consumer);

	return visit(k, context);
}

std::vector<DzResult> Emitter::visit(const fluenc::function_node *node, DefaultVisitorContext context) const
{
	Emitter nested;
	nested.function_ = function_;

	if (node->type == fluenc::function_type::exported)
	{
		auto module = context.entryPoint.module();
		auto llvmContext = context.entryPoint.context();

		auto returnType = type_for(node->signature.return_type, context.entryPoint);
		auto storageType = returnType->storageType(*llvmContext);

		auto arguments = node->signature.arguments;

		std::vector<llvm::Type *> argumentTypes;

		std::transform(begin(arguments), end(arguments), back_inserter(argumentTypes), [&](auto argument)
		{
			auto type = arg_type(argument, context.entryPoint);

			return type->storageType(*llvmContext);
		});

		auto functionType = llvm::FunctionType::get(storageType, argumentTypes, false);
		auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, node->signature.name, module);

		std::map<std::string, const BaseValue *> locals;

		std::transform(function->arg_begin(), function->arg_end(), begin(arguments), inserter(locals, begin(locals)), [&](auto &value, auto argument) -> std::pair<std::string, const BaseValue *>
		{
			auto standardArgument = std::get_if<const fluenc::default_argument *>(&argument);

			if (!standardArgument)
			{
				throw InvalidArgumentTypeException(nullptr);
			}

			auto type = type_for((*standardArgument)->type, context.entryPoint);

			return { (*standardArgument)->name, new scalar_value(type, &value) };
		});

		auto alloc = llvm::BasicBlock::Create(*llvmContext, "alloc");
		auto block = createBlock(llvmContext);

		linkBlocks(alloc, block);

		auto ep = context.entryPoint
			.withFunction(function)
			.withBlock(block)
			.withAlloc(alloc)
			.withLocals(locals);

		auto junc = new junction_node(node->block);

		std::vector<DzResult> result;

		for (auto &[rep, rva] : nested.visit(junc, { ep, context.values }))
		{
			auto llvmContext = rep.context();
			auto previous = rep.block();

			auto block = llvm::BasicBlock::Create(*llvmContext, "entry");

			linkBlocks(previous, block);

			auto ep = rep
				.withBlock(block);

			auto returnValue = ValueHelper::getScalar(node->ast, ep, rva);

			IRBuilderEx builder(ep);
			builder.createRet(*returnValue);

			result.push_back({ ep, rva });
		}

		ep.incorporate();

		llvm::FunctionAnalysisManager analysisManager;

		llvm::PassBuilder passBuilder;
		passBuilder.registerFunctionAnalyses(analysisManager);

	//	llvm::SimplifyCFGPass pass;
	//	pass.run(*function, analysisManager);

		verifyFunction(*function, &llvm::errs());

		return result;
	}

	if (node->type == fluenc::function_type::imported)
	{
		auto module = context.entryPoint.module();
		auto llvmContext = context.entryPoint.context();

		auto returnType = type_for(node->signature.return_type, context.entryPoint);

		std::vector<llvm::Type *> argumentTypes;
		std::vector<llvm::Value *> argumentValues;

		IRBuilderEx builder(context.entryPoint);

		for (const auto &argument : node->signature.arguments)
		{
			if (auto standardArgument = std::get_if<const fluenc::default_argument *>(&argument))
			{
				auto name = (*standardArgument)->name;
				auto type = type_for((*standardArgument)->type, context.entryPoint);

				auto storageType = type->storageType(*llvmContext);

				argumentTypes.push_back(storageType);

				auto value = context.values.pop();

				if (auto addressOfArgument = value_cast<const reference_value *>(value))
				{
					auto load = builder.createLoad(addressOfArgument, name);

					argumentValues.push_back(*load);
				}
				else if (auto stringValue = value_cast<const StringValue *>(value))
				{
					auto load = builder.createLoad(stringValue->reference());

					argumentValues.push_back(*load);
				}
				else if (auto bufferValue = value_cast<const BufferValue *>(value))
				{
					auto address = bufferValue->reference(context.entryPoint);

					argumentValues.push_back(*address);
				}
				else if (auto userTypeValue = value_cast<const user_type_value *>(value))
				{
					auto cast = InteropHelper::createWriteProxy(userTypeValue, context.entryPoint);

					argumentValues.push_back(cast);
				}
			}
			else
			{
				throw InvalidArgumentTypeException(node->ast);
			}
		}

		auto functionType = llvm::FunctionType::get(returnType->storageType(*llvmContext), argumentTypes, false);

		auto function = module->getOrInsertFunction(node->signature.name, functionType);

		auto call = builder.createCall(function, argumentValues);

			if (returnType->id() != type_id::Void)
		{
			auto [returnEntryPoint, returnValue] = InteropHelper::createReadProxy(call, returnType, context.entryPoint, node->ast);

			context.values.push(returnValue);
		}

		return {{ context.entryPoint, context.values }};
	}

	struct Argument
	{
		std::string name;

		const BaseValue *value;
	};

	auto handleArgument = [&](const fluenc::argument_t& argument, const BaseValue *value, auto &recurse)
	{
		return std::visit([&](auto *arg)  {
			using type = decltype(arg);

			if constexpr (std::is_same_v<type, const fluenc::tuple_argument*>)
			{
				auto tupleValue = value_cast<const tuple_value *>(value);

				auto tupleValues = tupleValue->values();

				std::vector<Argument> results;

				for (auto argument : arg->arguments)
				{
					for (auto &result : recurse(argument, tupleValues.pop(), recurse))
					{
						results.push_back(result);
					}
				}

				return results;
			}

			if constexpr (std::is_same_v<type, const fluenc::default_argument*>)
			{
				std::vector<Argument> result
				{
					{ arg->name, value }
				};

				if (auto userValue = value_cast<const user_type_value * >(value))
				{
					auto fields = userValue->fields();

					std::transform(begin(fields), end(fields), std::back_inserter(result), [=](auto field) -> Argument
					{
						std::stringstream ss;
						ss << arg->name;
						ss << ".";
						ss << field->name();

						return { ss.str(), field->value() };
					});
				}

				return result;
			}

			throw std::exception();
		}, argument);
	};

	std::vector<const Type *> types = { new iterator_type() };

	std::transform(context.values.begin(), context.values.end(), back_inserter(types), [](auto value)
	{
		return value->type();
	});

	auto pep = context.entryPoint
		.withValues(context.values)
				.withIteratorType(expanded_type::get(types));

	std::map<std::string, const BaseValue *> locals;

	for (const auto &argument : node->signature.arguments)
	{
		for (auto &[name, value] : handleArgument(argument, context.values.pop(), handleArgument))
		{
			locals[name] = value;
		}
	}

	auto ep = pep
		.withName(node->signature.name)
		.markEntry()
		.withLocals(locals);

	return fluenc::accept(node->block, nested, { ep, context.values });
}

std::vector<DzResult> Emitter::visit(const fluenc::global_node *node, DefaultVisitorContext context) const
{
	return fluenc::accept(node->value, *this, context);
}

std::vector<DzResult> Emitter::visit(const fluenc::return_node *node, DefaultVisitorContext context) const
{
	auto fetchValue = [&]() -> const BaseValue *
	{
		auto value = context.values.pop();

                if (auto typedValue = value_cast<const scalar_value *>(value))
		{
			IRBuilderEx builder(context.entryPoint);

			auto type = value->type();

			auto alloc = context.entryPoint.alloc(type);

			builder.createStore(typedValue, alloc);

			return alloc;
		}

		return value;
	};

	auto value = fetchValue();

	if (node->chained)
	{
		auto values = context.entryPoint.values();

		std::vector<const Type *> types = { iterator_type::get(node->iterator_) };

		std::transform(values.begin(), values.end(), back_inserter(types), [](auto value)
		{
			return value->type();
		});

		auto type = expanded_type::get(types);

		auto f = new Q();
		f->subject = *node->chained;
		f->function = function_;

		auto expandable = new ExpandableValue(false, type, context.entryPoint, f, values);
		auto tuple = new tuple_value({ expandable, value });

		context.values.push(tuple);

		return fluenc::accept(node->consumer, *this, context);
	}

	context.values.push(value);

	return fluenc::accept(node->consumer, *this, context);
}

std::vector<DzResult> Emitter::visit(const fluenc::terminator_node *node, DefaultVisitorContext context) const
{
	UNUSED(node);

	return {{ context.entryPoint, context.values }};
}

std::vector<DzResult> Emitter::visit(const ArrayValue *node, DefaultVisitorContext context) const
{
	auto llvmContext = context.entryPoint.context();
	auto block = context.entryPoint.block();

	std::vector<DzResult> results;

        auto index = context.values.require<reference_value>(node->m_ast);

	for (auto [_, elementValues] : node->m_values)
	{
		elementValues.push(index);

		auto arrayBlock = llvm::BasicBlock::Create(*llvmContext, "array");

		linkBlocks(block, arrayBlock);

		auto iteratorEntryPoint = context.entryPoint
			.withBlock(arrayBlock)
			.withName("__array")
			.markEntry()
			;

		for (auto &result : node->m_iterator->build(*this, { iteratorEntryPoint, elementValues }))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Emitter::visit(const IteratorValue *node, DefaultVisitorContext context) const
{
	auto locals = context.entryPoint.locals();

	for (auto &[key, value] : node->m_entryPoint->locals())
	{
		locals[key] = value;
	}

	auto ep = context.entryPoint
		.withLocals(locals);

	// return make_function_call(node->m_subject->ast, node->m_subject->names, { ep, Stack() });
	return fluenc::accept(node->m_subject, *this, { ep, Stack() });
}

std::vector<DzResult> Emitter::visit(const StringIteratable *node, DefaultVisitorContext context) const
{
	auto llvmContext = context.entryPoint.context();
	auto module = context.entryPoint.module();

	auto block = context.entryPoint.block();

	auto iteratorBlock = llvm::BasicBlock::Create(*llvmContext, "iterator");

	linkBlocks(block, iteratorBlock);

	auto iteratorEntryPoint = context.entryPoint
		.withBlock(iteratorBlock)
		.withName("__iterator")
		.markEntry();

	IRBuilderEx builder(iteratorEntryPoint);

	auto indexType = node->m_index->type();
	auto storageType = indexType->storageType(*llvmContext);

	auto ifTrue = createBlock(llvmContext);
	auto ifFalse = createBlock(llvmContext);

        auto lengthType = int64_type::instance();
	auto lengthStorageType = lengthType->storageType(*llvmContext);

	auto lengthLoad = builder.createLoad(node->m_length);

        auto constantOne = new scalar_value(lengthType
		, llvm::ConstantInt::get(lengthStorageType, 1)
		);

	auto length = builder.createSub(lengthLoad, constantOne);
	auto index = builder.createLoad(node->m_index);

	auto characterType = llvm::Type::getInt8Ty(*llvmContext);
	auto stringType = llvm::Type::getInt8PtrTy(*llvmContext);

	auto dataLayout = module->getDataLayout();

	auto align = dataLayout.getABITypeAlign(storageType);

	auto load = new llvm::LoadInst(stringType, *node->m_address, "strLoad", false, align, iteratorBlock);

        auto gep = new reference_value(byte_type::instance()
		, llvm::GetElementPtrInst::CreateInBounds(characterType, load, { *index }, "stringAccess", iteratorBlock)
		);

	auto value = builder.createLoad(gep);

        auto targetType = int32_type::instance();

        auto sext = new scalar_value(targetType
		, new llvm::SExtInst(*value, targetType->storageType(*llvmContext), "sext", iteratorBlock)
		);

	auto comparison = builder.createCmp(llvm::CmpInst::ICMP_SLT, index, length);

	builder.createCondBr(comparison , ifTrue, ifFalse);

	auto epIfFalse = iteratorEntryPoint
		.withBlock(ifFalse);

	auto epIfTrue = iteratorEntryPoint
		.withBlock(ifTrue);

        auto continuation = new ArrayContinuationNode(node->m_index, node->m_node, iterator_type::instance());
        auto expandable = new ExpandableValue(false, iterator_type::instance(), iteratorEntryPoint, continuation, node->m_index);

        auto tuple = new tuple_value({ expandable, sext });

	return
	{
		{ epIfTrue, tuple },
		{ epIfFalse, sext },
	};
}

std::vector<DzResult> Emitter::visit(const fluenc::tuple_sink_node *node, DefaultVisitorContext context) const
{
	std::vector<const BaseValue *> values;

	for (auto i = 0u; i < node->size; i++)
	{
		auto value = context.values.pop();

		values.insert(begin(values), value);
	}

	auto tuple = new tuple_value(values);

	context.values.push(tuple);

	return fluenc::accept(node->consumer, *this, context);
}

struct AllocResult
{
	const EntryPoint entryPoint;
	const BaseValue *value;
};

AllocResult alloc(const Type *type, const Emitter &visitor, const EntryPoint &entryPoint)
{
	if (auto userType = type_cast<const user_type *>(type))
	{
		auto elementTypes = userType->elementTypes();
		auto prototype = userType->prototype();

		auto fields = prototype->fields(entryPoint);

                std::vector<const named_value *> values;

		auto zipped = ranges::views::zip(fields, elementTypes);

		auto accumulatedEntryPoint = std::accumulate(begin(zipped), end(zipped), entryPoint, [&](auto currentEntryPoint, auto pair)
		{
			auto &[field, elementType] = pair;

			auto [allocEntryPoint, allocValue] = alloc(elementType, visitor, currentEntryPoint);

                        auto namedValue = new named_value(field.name, allocValue);

			values.push_back(namedValue);

			return allocEntryPoint;
		});

		return { accumulatedEntryPoint, new user_type_value(userType->prototype(), values) };
	}

	if (auto array = type_cast<const array_type *>(type))
	{
		auto types = array->types();

		std::vector<fluenc::indexed<const Type *>> indexed;

		transform(begin(types), end(types), fluenc::index_iterator(0), back_inserter(indexed), [&](auto type, auto index) -> fluenc::indexed<const Type *>
		{
			return { index, type };
		});

		auto firstValue = std::accumulate(begin(indexed), end(indexed), fluenc::expression_t { fluenc::terminator_node::instance() }, [&](auto next, auto pair)
		{
			auto indexSink = new fluenc::index_sink_node(pair.index, next);
			auto value = new DefaultNode(pair.value, indexSink);
			auto extension = new fluenc::extension_node(value);

			return extension;
		});

		fluenc::expression_t sink(new fluenc::array_sink_node(types.size(), {}, fluenc::terminator_node::instance(), firstValue));

		for (auto &result : fluenc::accept(sink, visitor, DefaultVisitorContext { entryPoint, Stack() }))
		{
			return { result.entryPoint, result.values.require<lazy_value>(nullptr) };
		}
	}

	if (auto string = type_cast<const string_type *>(type))
	{
		auto alloc = entryPoint.alloc(string);
                auto length = entryPoint.alloc(int64_type::instance());

		return { entryPoint, new StringValue(alloc, length) };
	}

        if (auto buffer = type_cast<const buffer_type *>(type))
	{
		auto alloc = entryPoint.alloc(buffer);

		return { entryPoint, new BufferValue(alloc) };
	}

        if (type->id() == type_id::Without)
	{
            return { entryPoint, without_value::instance() };
	}

        if (auto aggregate = type_cast<const aggregate_type *>(type))
	{
		std::vector<fluenc::function_node*> functions;

		auto subjects = aggregate->subjects();

		std::transform(begin(subjects), end(subjects), back_inserter(functions), [](const Type *subject)
		{
			if (auto function = type_cast<const function_type *>(subject))
			{
				return function->function();
			}
			else
			{
				throw InvalidTypeException(nullptr, "function", subject->name());
			}
		});

		return { entryPoint, new function_value(functions, entryPoint) };
	}

	return { entryPoint, entryPoint.alloc(type) };
}

std::vector<DzResult> Emitter::visit(const DefaultNode *node, DefaultVisitorContext context) const
{
	auto [entryPoint, value] = alloc(node->m_type, *this, context.entryPoint);

	context.values.push(value);

	return fluenc::accept(node->m_consumer, *this, { entryPoint, context.values });
}

std::vector<DzResult> Emitter::visit(const StaticNode *node, DefaultVisitorContext context) const
{
	context.values.push(node->m_value);

	std::vector<DzResult> results;

	for (auto &[entryPoint, values] : fluenc::accept(node->m_consumer, *this, context))
	{
		for (auto &result : node->m_next->build(*this, { entryPoint, values }))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Emitter::visit(const RenderedNode *node, DefaultVisitorContext context) const
{
	UNUSED(context);

	return std::any_cast<std::vector<DzResult>>(node->result);
}

std::vector<DzResult> Emitter::visit(const fluenc::extension_node *node, DefaultVisitorContext context) const
{
	if (node->data.type() == typeid(DefaultNode*))
	{
		return visit(std::any_cast<DefaultNode*>(node->data), context);
	}

	if (node->data.type() == typeid(RenderedNode*))
	{
		return visit(std::any_cast<RenderedNode*>(node->data), context);
	}

	if (node->data.type() == typeid(Foo*))
	{
		auto f = std::any_cast<Foo*>(node->data);

		return f->func(*this, context);
	}

	return {};
}
