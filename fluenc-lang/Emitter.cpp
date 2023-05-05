#include <llvm/IR/Verifier.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>
#include <llvm/Passes/PassBuilder.h>

#include <range/v3/view.hpp>

#include <unordered_map>
#include <unordered_set>

#include "Emitter.h"
#include "ValueHelper.h"
#include "IRBuilderEx.h"
#include "ITypeName.h"
#include "UndeclaredIdentifierException.h"
#include "FunctionNotFoundException.h"
#include "FunctionHelper.h"
#include "Indexed.h"
#include "IndexIterator.h"
#include "IPrototypeProvider.h"
#include "DzArgument.h"
#include "DzTupleArgument.h"
#include "InteropHelper.h"
#include "TypeCompatibilityCalculator.h"

#include "types/IOperatorSet.h"
#include "types/Int64Type.h"
#include "types/BooleanType.h"
#include "types/StringType.h"
#include "types/Int32Type.h"
#include "types/IPrototype.h"
#include "types/VoidType.h"
#include "types/ExpandedType.h"
#include "types/ByteType.h"
#include "types/ArrayType.h"

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
#include "values/IIteratable.h"
#include "values/IteratorValueGenerator.h"
#include "values/ArrayValueGenerator.h"
#include "values/PlaceholderValue.h"
#include "values/ArrayValue.h"
#include "values/IteratorValue.h"
#include "values/StringIteratable.h"
#include "values/IteratorValueGeneratorProxy.h"
#include "values/Iterator.h"
#include "values/ExpandedLazyValue.h"

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
#include "nodes/PreEvaluationNode.h"
#include "nodes/PostEvaluationNode.h"
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
#include "nodes/RenderedNode.h"
#include "nodes/Pod.h"
#include "nodes/ExportedFunctionTerminatorNode.h"
#include "nodes/AllocatorNode.h"
#include "nodes/TerminatorNode.h"

#include "exceptions/InvalidFunctionPointerTypeException.h"
#include "exceptions/MissingTailCallException.h"
#include "exceptions/AmbiguousFunctionException.h"
#include "exceptions/MissingDefaultValueException.h"
#include "exceptions/MissingFieldException.h"
#include "exceptions/InvalidArgumentTypeException.h"
#include "exceptions/InvalidOperatorException.h"
#include "exceptions/BinaryTypeMismatchException.h"

std::vector<DzResult> Emitter::visit(const BooleanBinaryNode *node, DefaultVisitorContext context) const
{
	auto left = ValueHelper::getScalar(node->ast, context.entryPoint, context.values);
	auto right = ValueHelper::getScalar(node->ast, context.entryPoint, context.values);

	IRBuilderEx builder(context.entryPoint);

	auto valueFactory = [&]
	{
		if (node->op == "==")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_EQ, left, right);
		}

		if (node->op == "!=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_NE, left, right);
		}

		if (node->op == "&&")
		{
			return builder.createLogicalAnd(left, right);
		}

		if (node->op == "||")
		{
			return builder.createLogicalOr(left, right);
		}

		auto operandType = left->type();
		auto operandTypeName = operandType->name();

		throw InvalidOperatorException(node->ast, node->op, operandTypeName);
	};

	auto value = valueFactory();

	context.values.push(value);

	return {{ context.entryPoint, context.values }};
}

std::vector<DzResult> Emitter::visit(const FloatBinaryNode *node, DefaultVisitorContext context) const
{
	auto left = ValueHelper::getScalar(node->ast, context.entryPoint, context.values);
	auto right = ValueHelper::getScalar(node->ast, context.entryPoint, context.values);

	IRBuilderEx builder(context.entryPoint);

	auto valueFactory = [&]
	{
		if (node->op == "+")
		{
			return builder.createFAdd(left, right);
		}

		if (node->op == "-")
		{
			return builder.createFSub(left, right);
		}

		if (node->op == "*")
		{
			return builder.createFMul(left, right);
		}

		if (node->op == "/")
		{
			return builder.createFDiv(left, right);
		}

		if (node->op == "<")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::FCMP_ULT, left, right);
		}

		if (node->op == "<=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::FCMP_ULE, left, right);
		}

		if (node->op == ">")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::FCMP_UGT, left, right);
		}

		if (node->op == ">=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::FCMP_UGE, left, right);
		}

		if (node->op == "==")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::FCMP_UEQ, left, right);
		}

		if (node->op == "!=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::FCMP_UNE, left, right);
		}

		auto operandType = left->type();
		auto operandTypeName = operandType->name();

		throw InvalidOperatorException(node->ast, node->op, operandTypeName);
	};

	auto value = valueFactory();

	context.values.push(value);

	return {{ context.entryPoint, context.values }};
}

std::vector<DzResult> Emitter::visit(const IntegerBinaryNode *node, DefaultVisitorContext context) const
{
	auto left = ValueHelper::getScalar(node->ast, context.entryPoint, context.values);
	auto right = ValueHelper::getScalar(node->ast, context.entryPoint, context.values);

	IRBuilderEx builder(context.entryPoint);

	auto valueFactory = [&]
	{
		if (node->op == "+")
		{
			return builder.createAdd(left, right);
		}

		if (node->op == "-")
		{
			return builder.createSub(left, right);
		}

		if (node->op == "*")
		{
			return builder.createMul(left, right);
		}

		if (node->op == "/")
		{
			return builder.createSDiv(left, right);
		}

		if (node->op == "<")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SLT, left, right);
		}

		if (node->op == "<=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SLE, left, right);
		}

		if (node->op == ">")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SGT, left, right);
		}

		if (node->op == ">=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SGE, left, right);
		}

		if (node->op == "==")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_EQ, left, right);
		}

		if (node->op == "!=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_NE, left, right);
		}

		if (node->op == "%")
		{
			return builder.createSRem(left, right);
		}

		if (node->op == "|")
		{
			return builder.createOr(left, right);
		}

		if (node->op == "&")
		{
			return builder.createAnd(left, right);
		}

		if (node->op == "^")
		{
			return builder.createXor(left, right);
		}

		auto operandType = left->type();
		auto operandTypeName = operandType->name();

		throw InvalidOperatorException(node->ast, node->op, operandTypeName);
	};

	auto value = valueFactory();

	context.values.push(value);

	return {{ context.entryPoint, context.values }};
}

std::vector<DzResult> Emitter::visit(const StringBinaryNode *node, DefaultVisitorContext context) const
{
	auto operand = context.values.pop();

	auto operandType = operand->type();
	auto operandTypeName = operandType->name();

	throw InvalidOperatorException(node->ast, node->op, operandTypeName);
}

std::vector<DzResult> Emitter::visit(const ArrayBinaryNode *node, DefaultVisitorContext context) const
{
	auto left = context.values.require<LazyValue>(node->ast);
	auto right = context.values.require<LazyValue>(node->ast);

	if (node->op == "|")
	{
		std::vector<const BaseValue *> inputs({ new ExpandedLazyValue(right), new ExpandedLazyValue(left) });

		auto generator = new AggregateIteratorValueGenerator(node, inputs);
		auto lazy = new LazyValue(generator, context.entryPoint);

		context.values.push(lazy);

		return node->consumer->accept(*this, { context.entryPoint, context.values });
	}

	auto operandType = left->type();
	auto operandTypeName = operandType->name();

	throw InvalidOperatorException(node->ast, node->op, operandTypeName);
}

std::vector<DzResult> Emitter::visit(const WithoutBinaryNode *node, DefaultVisitorContext context) const
{
	UNUSED(node);

	context.values.discard();

	return {{ context.entryPoint, context.values }};
}

std::vector<DzResult> Emitter::visit(const UserBinaryNode *node, DefaultVisitorContext context) const
{
	UNUSED(node);

	auto left = context.values.pop();
	auto right = context.values.pop();

	if (node->op == "|")
	{
		auto tuple = new TupleValue({ right, left });

		context.values.push(tuple);

		return {{ context.entryPoint, context.values }};
	}

	auto operandType = left->type();
	auto operandTypeName = operandType->name();

	throw InvalidOperatorException(node->ast, node->op, operandTypeName);
}

std::vector<DzResult> Emitter::visit(const BinaryNode *node, DefaultVisitorContext context) const
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

	for (auto &[resultEntryPoint, resultValues] : binary->accept(*this, context))
	{
		for (auto &result : node->consumer->accept(*this, { resultEntryPoint, resultValues }))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Emitter::visit(const ExportedFunctionNode *node, DefaultVisitorContext context) const
{
	auto module = context.entryPoint.module();
	auto llvmContext = context.entryPoint.context();

	auto returnType = node->m_returnType->resolve(context.entryPoint);
	auto storageType = returnType->storageType(*llvmContext);

	auto arguments = node->arguments();

	std::vector<llvm::Type *> argumentTypes;

	std::transform(begin(arguments), end(arguments), back_inserter(argumentTypes), [&](auto argument)
	{
		auto type = argument->type(context.entryPoint);

		return type->storageType(*llvmContext);
	});

	auto functionType = llvm::FunctionType::get(storageType, argumentTypes, false);
	auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, node->m_name, module);

	std::map<std::string, const BaseValue *> locals;

	std::transform(function->arg_begin(), function->arg_end(), begin(arguments), inserter(locals, begin(locals)), [&](auto &value, auto argument) -> std::pair<std::string, const BaseValue *>
	{
		auto standardArgument = dynamic_cast<const DzArgument *>(argument);

		if (!standardArgument)
		{
			throw InvalidArgumentTypeException(nullptr);
		}

		auto type = standardArgument->type(context.entryPoint);

		return { standardArgument->name(), new ScalarValue(type, &value) };
	});

	auto alloc = llvm::BasicBlock::Create(*llvmContext, "alloc");
	auto block = createBlock(llvmContext);

	linkBlocks(alloc, block);

	auto ep = context.entryPoint
		.withFunction(function)
		.withBlock(block)
		.withAlloc(alloc)
		.withLocals(locals);

	auto result = node->m_block->accept(*this, { ep, context.values });

	ep.incorporate();

	llvm::FunctionAnalysisManager analysisManager;

	llvm::PassBuilder passBuilder;
	passBuilder.registerFunctionAnalyses(analysisManager);

//	llvm::SimplifyCFGPass pass;
//	pass.run(*function, analysisManager);

	verifyFunction(*function, &llvm::errs());

	return result;
}

std::vector<DzResult> Emitter::visit(const ArrayContinuationNode *node, DefaultVisitorContext context) const
{
	auto llvmContext = context.entryPoint.context();

	auto indexType = Int64Type::instance();
	auto storageType = indexType->storageType(*llvmContext);

	auto indexConstant = new ScalarValue(indexType
		, llvm::ConstantInt::get(storageType, 1)
		);

	IRBuilderEx builder(context.entryPoint);

	auto load = builder.createLoad(node->m_index, "index");
	auto add = builder.createAdd(load, indexConstant);

	auto value = new ExpandedValue(true
		, node->m_iteratorType
		, context.entryPoint
		, node->m_node
		, node
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

	auto index = context.values.require<ReferenceValue>(node->m_ast);
	auto value = context.values.require<IndexedValue>(node->m_ast);

	if (node->m_next)
	{
		auto valuesIfFalse = context.values;

		auto indexType = index->type();
		auto storageType = indexType->storageType(*llvmContext);

		auto ifTrue = createBlock(llvmContext);
		auto ifFalse = createBlock(llvmContext);

		IRBuilderEx builder(context.entryPoint);

		auto indexLoad = builder.createLoad(index, "index");

		auto indexConstant = new ScalarValue(indexType
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

		auto continuation = new ArrayContinuationNode(index, node->m_node, IteratorType::instance());
		auto expandable = new ExpandableValue(true, node->m_arrayType, context.entryPoint, continuation, index);

		auto tuple = new TupleValue({ expandable, value->subject() });

		valuesIfTrue.push(tuple);

		auto resultsIfFalse = node->m_next->accept(*this, { epIfFalse, valuesIfFalse });

		std::vector<DzResult> result = {{ epIfTrue, valuesIfTrue }};

		result.insert(end(result), begin(resultsIfFalse), end(resultsIfFalse));

		return result;
	}

	auto ep = context.entryPoint
		.withIndex(value->index());

	context.values.push(value->subject());

	return {{ ep, context.values }};
}

std::vector<DzResult> Emitter::visit(const IntegralLiteralNode *node, DefaultVisitorContext context) const
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

std::vector<DzResult> Emitter::visit(const FloatLiteralNode *node, DefaultVisitorContext context) const
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

std::vector<DzResult> Emitter::visit(const BooleanLiteralNode *node, DefaultVisitorContext context) const
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

		throw std::exception(); // TODO
	};

	auto value = valueProvider();

	context.values.push(value);

	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Emitter::visit(const StringLiteralNode *node, DefaultVisitorContext context) const
{
	IRBuilderEx builder(context.entryPoint);

	auto stringType = StringType::get(node->m_value.size());

	auto string = new ScalarValue(stringType
		, builder.createGlobalStringPtr(node->m_value, "string")
		);

	auto alloc = context.entryPoint.alloc(stringType);

	builder.createStore(string, alloc);

	context.values.push(new StringValue(alloc, node->m_value.size()));

	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Emitter::visit(const CharacterLiteralNode *node, DefaultVisitorContext context) const
{
	auto llvmContext = context.entryPoint.context();

	auto type = Int32Type::instance();
	auto storageType = type->storageType(*llvmContext);

	auto value = new ScalarValue(type
		, llvm::ConstantInt::get((llvm::IntegerType *)storageType
			, *node->m_value.begin()
			)
		);

	context.values.push(value);

	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Emitter::visit(const NothingNode *node, DefaultVisitorContext context) const
{
	context.values.push(WithoutValue::instance());

	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Emitter::visit(const MemberAccessNode *node, DefaultVisitorContext context) const
{
	auto &locals = context.entryPoint.locals();
	auto &functions = context.entryPoint.functions();
	auto &globals = context.entryPoint.globals();

	for (auto &name : node->m_names)
	{
		auto localsIterator = locals.find(name);

		if (localsIterator != locals.end())
		{
			if (auto referenceValue = value_cast<const ReferenceValue *>(localsIterator->second))
			{
				IRBuilderEx builder(context.entryPoint);

				auto load = builder.createLoad(referenceValue);

				context.values.push(load);
			}
			else if (localsIterator->second)
			{
				context.values.push(localsIterator->second);
			}

			return node->m_consumer->accept(*this, context);
		}

		auto functionsIterator = functions.equal_range(name);

		std::vector<const CallableNode *> matchingFunctions;

		std::transform(functionsIterator.first, functionsIterator.second, back_inserter(matchingFunctions), [](auto pair)
		{
			return pair.second;
		});

		if (matchingFunctions.size() > 0)
		{
			auto value = new FunctionValue(matchingFunctions, context.entryPoint);

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

	throw UndeclaredIdentifierException(node->m_ast, node->m_names[0]);
}

std::vector<DzResult> Emitter::visit(const ReferenceSinkNode *node, DefaultVisitorContext context) const
{
	auto makeReference = [&](const BaseValue *value, auto &recurse) -> const BaseValue *
	{
		IRBuilderEx builder(context.entryPoint);

		if (auto typedValue = value_cast<const ScalarValue *>(value))
		{
			auto argumentType = typedValue->type();

			auto alloc = context.entryPoint.alloc(argumentType);

			builder.createStore(typedValue, alloc);

			return alloc;
		}

		if (auto referenceValue = value_cast<const ReferenceValue *>(value))
		{
			return referenceValue->clone(context.entryPoint, CloneStrategy::Value);
		}

		if (auto tupleValue = value_cast<const TupleValue *>(value))
		{
			auto tupleValues = tupleValue->values();

			std::vector<const BaseValue *> values;

			std::transform(tupleValues.begin(), tupleValues.end(), std::back_inserter(values), [&](auto value)
			{
				return recurse(value, recurse);
			});

			return new TupleValue(values);
		}

		if (auto userTypeValue = value_cast<const UserTypeValue *>(value))
		{
			auto fields = userTypeValue->fields();

			std::vector<const NamedValue *> values;

			std::transform(begin(fields), end(fields), std::back_inserter(values), [&](auto field)
			{
				return new NamedValue { field->name(), recurse(field->value(), recurse) };
			});

			return new UserTypeValue { userTypeValue->prototype(), values };
		}

		return value;
	};

	auto value = makeReference(context.values.pop(), makeReference);

	context.values.push(value);

	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Emitter::visit(const PreEvaluationNode *node, DefaultVisitorContext context) const
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
		for (auto &result : node->m_consumer->accept(*this, { resultEntryPoint, resultValues }))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Emitter::visit(const PostEvaluationNode *node, DefaultVisitorContext context) const
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

			if (auto tuple = value_cast<const TupleValue *>(value))
			{
				for (auto &element : tuple->values())
				{
					if (auto expanded = value_cast<const ExpandedValue *>(element))
					{
						std::vector<DzResult> results;

						auto node = expanded->node();

						auto pre = new PreEvaluationNode(TerminatorNode::instance());

						for (auto &[rep, rva] : pre->accept(*this, { entryPoint, expanded->values() }))
						{
							for (auto &[inputEntryPoint, inputValues] : recurse(rep, rva, recurse))
							{
								for (auto &[resultEntryPoint, resultValues] : node->accept(*this, { inputEntryPoint, inputValues }))
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

	return digestDepth(context.entryPoint, context.values, digestDepth);
}

std::vector<DzResult> Emitter::visit(const FunctionCallNode *node, DefaultVisitorContext context) const
{
	auto functionsForName = [&](const std::string &name)
	{
		auto &functions = context.entryPoint.functions();
		auto &locals = context.entryPoint.locals();

		auto local = locals.find(name);

		if (local != locals.end())
		{
			auto value = value_cast<const FunctionValue *>(local->second);

			if (!value)
			{
				throw InvalidFunctionPointerTypeException(node->m_ast, name);
			}

			return value->functions();
		}

		auto matchingFunctions = functions.equal_range(name);

		std::vector<const CallableNode *> result;

		std::transform(matchingFunctions.first, matchingFunctions.second, back_inserter(result), [](auto pair)
		{
			return pair.second;
		});

		return result;
	};

	auto findFunction = [&](const std::vector<const Type *> &types) -> const CallableNode *
	{
		for (auto &name : node->m_names)
		{
			std::map<int8_t, const CallableNode *> candidates;

			for (auto &function : functionsForName(name))
			{
				auto score = function->signatureCompatibility(context.entryPoint, types);

				if (score < 0)
				{
					continue;
				}

				auto candidate = candidates.find(score);

				if (candidate != candidates.end())
				{
					std::vector<const CallableNode *> functions = { candidate->second, function };

					throw AmbiguousFunctionException(node->m_ast
						, functions
						, context.entryPoint
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

	auto [score, _1, _2] = FunctionHelper::tryCreateTailCall(context.entryPoint, context.values, begin(node->m_names), end(node->m_names));

	if (score == 0)
	{
		throw MissingTailCallException(node->m_ast);
	}

	auto llvmContext = context.entryPoint.context();

	std::vector<DzResult> result;

	for (auto &[resultEntryPoint, resultValues] : node->m_evaluation->accept(*this, context))
	{
		auto block = resultEntryPoint.block();

		std::vector<const Type *> types;

		std::transform(resultValues.rbegin(), resultValues.rend(), std::back_inserter(types), [](auto value)
		{
			return value->type();
		});

		auto function = findFunction(types);

		if (!function)
		{
			throw FunctionNotFoundException(node->m_ast, node->m_names[0], types);
		}

		auto functionBlock = createBlock(llvmContext);

		linkBlocks(block, functionBlock);

		auto functionEntryPoint = resultEntryPoint
			.withBlock(functionBlock);

		if (function->attribute() == FunctionAttribute::Import)
		{
			return function->accept(*this, { functionEntryPoint, resultValues });
		}

		auto functionResults = function->accept(*this, { functionEntryPoint, resultValues });

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

std::vector<DzResult> Emitter::visit(const StackSegmentNode *node, DefaultVisitorContext context) const
{
	std::vector<DzResult> result;
	std::vector<DzResult> input = {{ context.entryPoint.withIndex(-1), Stack() }};

	std::vector<Indexed<Node *>> orderedValues;

	std::transform(begin(node->m_values), end(node->m_values), index_iterator(), std::back_inserter(orderedValues), [](auto value, auto index) -> Indexed<Node *>
	{
		return { index, value };
	});

	std::sort(begin(orderedValues), end(orderedValues), [=](auto first, auto second)
	{
		return first.value->order(context.entryPoint) < second.value->order(context.entryPoint);
	});

	auto subjectResults = std::accumulate(begin(orderedValues), end(orderedValues), input, [&](auto accumulator, auto argument)
	{
		std::vector<DzResult> results;

		for (auto &[accumulatorEntryPoint, accumulatorValues] : accumulator)
		{
			auto result = argument.value->accept(*this, { accumulatorEntryPoint, Stack() });

			for (auto &[resultEntryPoint, resultValues] : result)
			{
				auto scopedReturnValues = accumulatorValues;

				for (auto resultValue : resultValues)
				{
					auto returnValue = new IndexedValue{ argument.index, resultValue };

					scopedReturnValues.push(returnValue);
				}

				results.push_back({ resultEntryPoint, scopedReturnValues });
			}
		}

		return results;
	});

	for (auto &[subjectEntryPoint, subjectValues] : subjectResults)
	{
		std::multimap<int, const BaseValue *, std::greater<int>> indexedValues;

		for (auto &value : subjectValues)
		{
			auto indexed = static_cast<const IndexedValue *>(value);

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

		auto pod = new Pod(node->m_call, context.values);
		auto callResults = pod->accept(*this, { preliminaryEntryPoint, pointersToValues });

		auto producesIterator = std::any_of(begin(callResults), end(callResults), [](auto pair)
		{
			auto &[_, callValues] = pair;

			if (callValues.size() <= 0)
			{
				return false;
			}

			auto returnValue = callValues.peek();

			return returnValue->id() == ValueId::Tuple;
		});

		if (producesIterator)
		{
			auto iterator = new Iterator(pod, new IteratorType(), preliminaryEntryPoint, pointersToValues, callResults);

			result.push_back({ subjectEntryPoint, iterator });
		}
		else
		{
			linkBlocks(subjectEntryPoint.block(), preliminaryBlock);

			std::vector<DzResult> preliminaryResults;

			for (auto &[callEntryPoint, callValues] : callResults)
			{
				auto consumerResults = node->m_consumer->accept(*this, { callEntryPoint, callValues });

				for (auto &consumerResult : consumerResults)
				{
					result.push_back(consumerResult);
				}
			}
		}
	}

	return result;
}

std::vector<DzResult> Emitter::visit(const FunctionCallProxyNode *node, DefaultVisitorContext context) const
{
	auto llvmContext = context.entryPoint.context();

	std::vector<DzResult> results;

	auto preliminaryBlock = createBlock(llvmContext);

	auto preliminaryEntryPoint = new EntryPoint(context.entryPoint
		.withBlock(preliminaryBlock)
		);

	auto preliminaryResults = node->m_regularCall->accept(*this, { preliminaryEntryPoint->detach(), context.values });

	for (auto &[_, preliminaryValues] : preliminaryResults)
	{
		if (preliminaryValues.size() <= 0)
		{
			continue;
		}

		auto returnValue = preliminaryValues.peek();

		if (returnValue->id() == ValueId::Iterator)
		{
			auto subject = new IteratorValueGenerator(new IteratorType(), node->m_regularCall, context.entryPoint);
			auto generator = new IteratorValueGeneratorProxy(subject, preliminaryEntryPoint, preliminaryResults);
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

	preliminaryEntryPoint->setParent(context.entryPoint);

	linkBlocks(context.entryPoint.block(), preliminaryBlock);

	auto rendered = new RenderedNode(preliminaryResults);
	auto junction = new JunctionNode(rendered);

	for (auto &[resultEntryPoint, resultValues] : junction->accept(*this, context))
	{
		auto consumerEntryPoint = resultEntryPoint
			.withIndex(context.entryPoint.index());

		for (auto &result : node->m_consumer->accept(*this, { consumerEntryPoint, resultValues }))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Emitter::visit(const JunctionNode *node, DefaultVisitorContext context) const
{
	struct type_comparer
	{
		bool operator()(const Type *x, const Type *y) const
		{
			if (x == y)
			{
				return false;
			}

			if (x->id() == TypeId::Tuple)
			{
				return true;
			}

			if (y->id() == TypeId::Tuple)
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

		auto alloc = first->clone(entryPoint, CloneStrategy::Storage);

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

	auto inputResults = node->m_subject->accept(*this, context);

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

std::vector<DzResult> Emitter::visit(const InstantiationNode *node, DefaultVisitorContext context) const
{
	auto llvmContext = context.entryPoint.context();
	auto module = context.entryPoint.module();

	auto block = context.entryPoint.block();

	auto dataLayout = module->getDataLayout();

	std::unordered_map<std::string, const BaseValue *> valuesByName;

	std::transform(begin(node->m_fields), end(node->m_fields), std::inserter(valuesByName, begin(valuesByName)), [&](auto field)
	{
		return std::make_pair(field, context.values.pop());
	});

	auto prototype = node->m_prototypeProvider->provide(context.entryPoint, context.values);

	auto [pep, prototypeFields] = prototype->fields(context.entryPoint, *this);

	auto prototypeEntryPoint = pep;

	std::vector<const NamedValue *> namedValues;

	std::transform(begin(prototypeFields), end(prototypeFields), std::back_inserter(namedValues), [&](auto field) -> const NamedValue *
	{
		auto valueByName = valuesByName.find(field.name());

		if (valueByName != valuesByName.end())
		{
			auto value = valueByName->second;

			valuesByName.erase(valueByName);

			if (auto reference = value_cast<const ReferenceValue *>(field.defaultValue()))
			{
				if (reference->type() != value->type())
				{
					return new NamedValue { field.name(), value };
				}

				auto type = reference->type();
				auto storageType = type->storageType(*llvmContext);

				auto align = dataLayout.getABITypeAlign(storageType);

				auto typedValue = ValueHelper::getScalar(node->m_ast, context.entryPoint, value);

				auto store = new llvm::StoreInst(*typedValue, *reference, false, align, block);

				UNUSED(store);

				return new NamedValue { field.name(), field.defaultValue() };
			}

			return new NamedValue { field.name(), value };
		}

		if (!field.defaultValue())
		{
			throw MissingDefaultValueException(node->m_ast, field.name());
		}

		return new NamedValue { field.name(), field.defaultValue() };
	});

	for (auto &[name, _] : valuesByName)
	{
		throw MissingFieldException(node->m_ast, prototype->name(), name);
	}

	IRBuilderEx builder(prototypeEntryPoint);

	std::vector<const NamedValue *> finalValues;

	std::transform(begin(namedValues), end(namedValues), std::back_inserter(finalValues), [&](auto field) -> const NamedValue *
	{
		auto value = field->value();

		if (auto typedValue = value_cast<const ScalarValue *>(value))
		{
			auto type = typedValue->type();

			auto alloc = prototypeEntryPoint.alloc(type);

			builder.createStore(typedValue, alloc);

			return new NamedValue { field->name(), alloc };
		}

		return field;
	});

	auto userTypeValue = new UserTypeValue(prototype, finalValues);

	context.values.push(userTypeValue);

	return node->m_consumer->accept(*this, { prototypeEntryPoint, context.values });
}

std::vector<DzResult> Emitter::visit(const ConditionalNode *node, DefaultVisitorContext context) const
{
	struct SingleResult
	{
		const EntryPoint entryPoint;
		const ScalarValue *value;
	};

	auto llvmContext = context.entryPoint.context();
	auto module = context.entryPoint.module();

	auto block = context.entryPoint.block();

	auto dataLayout = module->getDataLayout();

	block->setName("condition");

	auto ifTrue = createBlock(llvmContext);
	auto ifFalse = createBlock(llvmContext);

	IRBuilderEx builder(context.entryPoint);

	auto conditional = ValueHelper::getScalar(node->m_ast, context.entryPoint, context.values);

	builder.createCondBr(conditional, ifTrue, ifFalse);

	auto epIfFalse = context.entryPoint
		.withName("ifFalse")
		.withBlock(ifFalse);

	auto epIfTrue = context.entryPoint
		.withName("ifTrue")
		.withBlock(ifTrue);

	auto resultsIfTrue = node->m_ifTrue->accept(*this, { epIfTrue, context.values });
	auto resultsIfFalse = node->m_ifFalse->accept(*this, { epIfFalse, context.values });

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

		auto value = resultValues.request<ScalarValue>();

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

std::vector<DzResult> Emitter::visit(const BlockInstructionNode *node, DefaultVisitorContext context) const
{
	return node->m_subject->accept(*this, context);
}

std::vector<DzResult> Emitter::visit(const EmptyArrayNode *node, DefaultVisitorContext context) const
{
	context.values.push(WithoutValue::instance());

	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Emitter::visit(const IndexSinkNode *node, DefaultVisitorContext context) const
{
	auto value = context.values.pop();

	auto indexed = new IndexedValue(node->m_index, value);

	context.values.push(indexed);

	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Emitter::visit(const ArraySinkNode *node, DefaultVisitorContext context) const
{
	auto arrayContents = node->m_firstValue->accept(*this, { context.entryPoint, Stack() });

	auto generator = new ArrayValueGenerator(arrayContents, node->m_ast, node->id(), node->m_size);
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

std::vector<DzResult> Emitter::visit(const ExpansionNode *node, DefaultVisitorContext context) const
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

		auto result = continuation->accept(*this, { continuationEntryPoint, Stack() });

		for (auto &[targetEntryPoint, continuationValues] : result)
		{
			auto value = continuationValues
				.require<ExpandedValue>(nullptr);

			auto tuple = new TupleValue({ value, PlaceholderValue::instance() });

			context.values.push(tuple);

			auto consumerEntryPoint = context.entryPoint
				.withBlock(targetEntryPoint.block());

			return node->m_consumer->accept(*this, { consumerEntryPoint, context.values });
		}

		throw std::exception();
	}
	else if (auto lazy = value_cast<const LazyValue *>(value))
	{
		context.values.push(new ExpandedLazyValue(lazy));

		return node->m_consumer->accept(*this, context);
	}
	else if (auto string = value_cast<const StringValue *>(value))
	{
		context.values.push(new ExpandedLazyValue(string->iterator(context.entryPoint)));

		return node->m_consumer->accept(*this, context);
	}
	else
	{
		context.values.push(value);

		return node->m_consumer->accept(*this, context);
	}
}

std::vector<DzResult> Emitter::visit(const LocalNode *node, DefaultVisitorContext context) const
{
	auto locals = context.entryPoint.locals();

	auto value = context.values.pop();

	if (auto userValue = value_cast<const UserTypeValue * >(value))
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

	if (auto lazyValue = value_cast<const LazyValue *>(value))
	{
		auto type = lazyValue->type();

		// Optimization: If iteration can be represented as an array, create
		// a new, temporary array and copy the iteration result into it.
		// This will ensure that multiple iterations are cheap.
		//
		if (type->id() == TypeId::Array)
		{
			auto allocator = new AllocatorNode(type, TerminatorNode::instance());

			for (auto &result : allocator->accept(*this, { context.entryPoint, Stack() }))
			{
				auto array = result.values.require<LazyValue>(nullptr);

				auto assignmentEntryPoint = array->assignFrom(result.entryPoint, lazyValue, *this);

				locals[node->m_name] = array;

				auto ep = assignmentEntryPoint
					.withLocals(locals);

				return node->m_consumer->accept(*this, { ep, context.values });
			}
		}
	}

	locals[node->m_name] = value;

	auto ep = context.entryPoint
		.withLocals(locals);

	return node->m_consumer->accept(*this, { ep, context.values });
}

std::vector<DzResult> Emitter::visit(const ContinuationNode *node, DefaultVisitorContext context) const
{
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
		return value->clone(context.entryPoint, CloneStrategy::Value);
	});

	auto next = ValueHelper::extractValues<ExpandedValue>(context.values);

	auto isArray = accumulate(begin(next), end(next), next.size() > 0, [](auto accumulated, auto value)
	{
		return accumulated && value->isArray();
	});

	auto value = new ExpandedValue(isArray
		, ExpandedType::get(types)
		, context.entryPoint
		, node->m_node
		, node
		, next
		, cloned
		);

	return {{ context.entryPoint, value }};
}

std::vector<DzResult> Emitter::visit(const BooleanUnaryNode *node, DefaultVisitorContext context) const
{
	auto operand = ValueHelper::getScalar(node->ast, context.entryPoint, context.values);

	IRBuilderEx builder(context.entryPoint);

	auto valueFactory = [&]
	{
		if (node->op == "!")
		{
			return builder.createNot(operand);
		}

		auto operandType = operand->type();
		auto operandTypeName = operandType->name();

		throw InvalidOperatorException(node->ast, node->op, operandTypeName);
	};

	auto value = valueFactory();

	context.values.push(value);

	return {{ context.entryPoint, context.values }};
}

std::vector<DzResult> Emitter::visit(const FloatUnaryNode *node, DefaultVisitorContext context) const
{
	auto operand = context.values.pop();

	auto operandType = operand->type();
	auto operandTypeName = operandType->name();

	throw InvalidOperatorException(node->ast, node->op, operandTypeName);
}

std::vector<DzResult> Emitter::visit(const IntegerUnaryNode *node, DefaultVisitorContext context) const
{
	auto operand = context.values.pop();

	auto operandType = operand->type();
	auto operandTypeName = operandType->name();

	throw InvalidOperatorException(node->ast, node->op, operandTypeName);
}

std::vector<DzResult> Emitter::visit(const StringUnaryNode *node, DefaultVisitorContext context) const
{
	auto operand = context.values.pop();

	auto valueFactory = [&]
	{
		if (node->op == "@")
		{
			return new ForwardedValue(operand);
		}

		auto operandType = operand->type();
		auto operandTypeName = operandType->name();

		throw InvalidOperatorException(node->ast, node->op, operandTypeName);
	};

	auto value = valueFactory();

	context.values.push(value);

	return node->consumer->accept(*this, context);
}

std::vector<DzResult> Emitter::visit(const ArrayUnaryNode *node, DefaultVisitorContext context) const
{
	auto operand = context.values.pop();

	auto operandType = operand->type();
	auto operandTypeName = operandType->name();

	throw InvalidOperatorException(node->ast, node->op, operandTypeName);
}

std::vector<DzResult> Emitter::visit(const WithoutUnaryNode *node, DefaultVisitorContext context) const
{
	UNUSED(node);

	return {{ context.entryPoint, context.values }};
}

std::vector<DzResult> Emitter::visit(const UserUnaryNode *node, DefaultVisitorContext context) const
{
	auto operand = context.values.pop();

	auto operandType = operand->type();
	auto operandTypeName = operandType->name();

	throw InvalidOperatorException(node->ast, node->op, operandTypeName);
}

std::vector<DzResult> Emitter::visit(const UnaryNode *node, DefaultVisitorContext context) const
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

	for (auto &[resultEntryPoint, resultValues] : unary->accept(*this, context))
	{
		for (auto &result : node->consumer->accept(*this, { resultEntryPoint, resultValues }))
		{
			results.push_back(result);
		}
	}

	return results;
}

#include <iostream>

std::vector<DzResult> Emitter::visit(const TailFunctionCallNode *node, DefaultVisitorContext context) const
{
	auto [score, tailCallTarget, targetValues] = FunctionHelper::tryCreateTailCall(context.entryPoint, context.values, begin(node->m_names), end(node->m_names));

	if (score == 0)
	{
		std::vector<const BaseValue *> cloned;

		std::transform(context.values.begin(), context.values.end(), back_inserter(cloned), [&](auto value)
		{
			return value->clone(context.entryPoint, CloneStrategy::Value);
		});

		auto zipped = ranges::views::zip(cloned, targetValues);

		auto resultEntryPoint = std::accumulate(zipped.begin(), zipped.end(), context.entryPoint, [&](auto accumulatedEntryPoint, auto result)
		{
			auto [value, storage] = result;

			return ValueHelper::transferValue(accumulatedEntryPoint, value, storage, *this);
		});

		linkBlocks(resultEntryPoint.block(), tailCallTarget->block());

		return std::vector<DzResult>();
	}

	if (score == 1)
	{
		throw std::exception(); // TODO
	}

	auto junction = new JunctionNode(node->m_regularCall);

	return junction->accept(*this, context);
}

std::vector<DzResult> Emitter::visit(const FunctionNode *node, DefaultVisitorContext context) const
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

			if (auto userValue = value_cast<const UserTypeValue * >(value))
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
			auto tupleValue = value_cast<const TupleValue *>(value);

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

		throw std::exception();
	};

	std::vector<const Type *> types = { new IteratorType() };

	std::transform(context.values.begin(), context.values.end(), back_inserter(types), [](auto value)
	{
		return value->type();
	});

	auto pep = context.entryPoint
		.withValues(context.values)
		.withIteratorType(ExpandedType::get(types));

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
		.withLocals(locals);

	return node->m_block->accept(*this, { ep, context.values });
}

std::vector<DzResult> Emitter::visit(const ExportedFunctionTerminatorNode *node, DefaultVisitorContext context) const
{
	UNUSED(node);

	auto llvmContext = context.entryPoint.context();
	auto previous = context.entryPoint.block();

	auto block = llvm::BasicBlock::Create(*llvmContext, "entry");

	linkBlocks(previous, block);

	auto ep = context.entryPoint
		.withBlock(block);

	auto returnValue = ValueHelper::getScalar(node->m_ast, ep, context.values);

	IRBuilderEx builder(ep);
	builder.createRet(*returnValue);

	return {{ ep, context.values }};
}

std::vector<DzResult> Emitter::visit(const ImportedFunctionNode *node, DefaultVisitorContext context) const
{
	auto module = context.entryPoint.module();
	auto llvmContext = context.entryPoint.context();

	auto returnType = node->m_returnType->resolve(context.entryPoint);

	std::vector<llvm::Type *> argumentTypes;
	std::vector<llvm::Value *> argumentValues;

	IRBuilderEx builder(context.entryPoint);

	for (const auto &argument : node->m_arguments)
	{
		if (auto standardArgument  = dynamic_cast<DzArgument *>(argument))
		{
			auto name = standardArgument->name();
			auto type = standardArgument->type(context.entryPoint);

			auto storageType = type->storageType(*llvmContext);

			argumentTypes.push_back(storageType);

			auto value = context.values.pop();

			if (auto addressOfArgument = value_cast<const ReferenceValue *>(value))
			{
				auto load = builder.createLoad(addressOfArgument, name);

				argumentValues.push_back(*load);
			}
			else if (auto stringValue = value_cast<const StringValue *>(value))
			{
				auto load = builder.createLoad(stringValue->reference());

				argumentValues.push_back(*load);
			}
			else if (auto userTypeValue = value_cast<const UserTypeValue *>(value))
			{
				auto cast = InteropHelper::createWriteProxy(userTypeValue, context.entryPoint);

				argumentValues.push_back(cast);
			}
		}
		else
		{
			throw InvalidArgumentTypeException(node->m_ast);
		}
	}

	llvm::FunctionType *functionType = llvm::FunctionType::get(returnType->storageType(*llvmContext), argumentTypes, false);

	auto function = module->getOrInsertFunction(node->m_name, functionType);

	auto call = builder.createCall(function, argumentValues);

	if (returnType != VoidType::instance())
	{
		auto [returnEntryPoint, returnValue] = InteropHelper::createReadProxy(call, returnType, context.entryPoint, node->m_ast);

		context.values.push(returnValue);

		return {{ returnEntryPoint, context.values }};
	}

	return {{ context.entryPoint, context.values }};
}

std::vector<DzResult> Emitter::visit(const GlobalNode *node, DefaultVisitorContext context) const
{
	return node->m_value->accept(*this, context);
}

std::vector<DzResult> Emitter::visit(const ReturnNode *node, DefaultVisitorContext context) const
{
	auto fetchValue = [&]() -> const BaseValue *
	{
		auto value = context.values.pop();

		if (auto typedValue = value_cast<const ScalarValue *>(value))
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

	if (node->m_chained)
	{
		auto values = context.entryPoint.values();

		std::vector<const Type *> types = { node->m_iteratorType };

		std::transform(values.begin(), values.end(), back_inserter(types), [](auto value)
		{
			return value->type();
		});

		auto type = ExpandedType::get(types);

		auto expandable = new ExpandableValue(false, type, context.entryPoint, node->m_chained, values);
		auto tuple = new TupleValue({ expandable, value });

		context.values.push(tuple);

		return node->m_consumer->accept(*this, context);
	}

	context.values.push(value);

	return node->m_consumer->accept(*this, context);
}

std::vector<DzResult> Emitter::visit(const ParentInjectorNode *node, DefaultVisitorContext context) const
{
	return node->m_subject->accept(*this, context);
}

std::vector<DzResult> Emitter::visit(const BlockStackFrameNode *node, DefaultVisitorContext context) const
{
	return node->m_consumer->accept(*this, { context.entryPoint, Stack() });
}

std::vector<DzResult> Emitter::visit(const TerminatorNode *node, DefaultVisitorContext context) const
{
	UNUSED(node);

	return {{ context.entryPoint, context.values }};
}

std::vector<DzResult> Emitter::visit(const IteratableNode *node, DefaultVisitorContext context) const
{
	auto index = context.values.require<ScalarValue>(nullptr);
	auto alloc = context.entryPoint.alloc(Int64Type::instance());

	IRBuilderEx builder(context.entryPoint);

	builder.createStore(index, alloc);

	context.values.push(alloc);

	return node->m_iteratable->accept(*this, context);
}

std::vector<DzResult> Emitter::visit(const ArrayValue *node, DefaultVisitorContext context) const
{
	auto llvmContext = context.entryPoint.context();
	auto block = context.entryPoint.block();

	std::vector<DzResult> results;

	auto index = context.values.require<ReferenceValue>(node->m_ast);

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

		for (auto &result : node->m_iterator->accept(*this, { iteratorEntryPoint, elementValues }))
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

	return node->m_subject->accept(*this, { ep, Stack() });
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

	auto length = new ScalarValue(indexType
		, llvm::ConstantInt::get(storageType, node->m_length - 1)
		);

	auto index = builder.createLoad(node->m_index);

	auto characterType = llvm::Type::getInt8Ty(*llvmContext);
	auto stringType = llvm::Type::getInt8PtrTy(*llvmContext);

	auto dataLayout = module->getDataLayout();

	auto align = dataLayout.getABITypeAlign(storageType);

	auto load = new llvm::LoadInst(stringType, node->m_address, "strLoad", false, align, iteratorBlock);

	auto gep = new ReferenceValue(ByteType::instance()
		, llvm::GetElementPtrInst::CreateInBounds(characterType, load, { *index }, "stringAccess", iteratorBlock)
		);

	auto value = builder.createLoad(gep);

	auto targetType = Int32Type::instance();

	auto sext = new ScalarValue(targetType
		, new llvm::SExtInst(*value, targetType->storageType(*llvmContext), "sext", iteratorBlock)
		);

	auto comparison = builder.createCmp(llvm::CmpInst::ICMP_SLT, index, length);

	builder.createCondBr(comparison , ifTrue, ifFalse);

	auto epIfFalse = iteratorEntryPoint
		.withBlock(ifFalse);

	auto epIfTrue = iteratorEntryPoint
		.withBlock(ifTrue);

	auto continuation = new ArrayContinuationNode(node->m_index, node->m_node, IteratorType::instance());
	auto expandable = new ExpandableValue(false, IteratorType::instance(), iteratorEntryPoint, continuation, node->m_index);

	auto tuple = new TupleValue({ expandable, sext });

	return
	{
		{ epIfTrue, tuple },
		{ epIfFalse, sext },
	};
}

std::vector<DzResult> Emitter::visit(const DistributorNode *node, DefaultVisitorContext context) const
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
