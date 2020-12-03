#include "VisitorV1.h"

VisitorV1::VisitorV1(LLVMContext &context, Module &module, BasicBlock *block, Function *function, std::map<std::string, Value *> locals)
	: m_context(context)
	, m_module(module)
	, m_block(block)
	, m_function(function)
	, m_locals(locals)
{
}

antlrcpp::Any VisitorV1::visitAssignment(dzParser::AssignmentContext *context)
{
	IRBuilder<> builder(m_block);

	auto name = context->ID()->getText();
	auto expression = context->expression();

	auto value = visitAs<Value *>(expression);

	auto alloc = builder.CreateAlloca(value->getType());

	builder.CreateStore(value, alloc);

	return builder.CreateLoad(alloc, name);
}

antlrcpp::Any VisitorV1::visitFunction(dzParser::FunctionContext *context)
{
	auto returnType = visitAs<Type *>(context->typeName());
	auto arguments = context->argument();
	auto name = context->ID()->getText();

	std::vector<Type *> argumentTypes;
	std::map<std::string, Value *> locals;

	for (auto argument : arguments)
	{
		auto argumentType = visitAs<Type *>(argument->typeName());

		argumentTypes.push_back(argumentType);
	}

	auto functionType = FunctionType::get(returnType, argumentTypes, false);
	auto function = Function::Create(functionType, Function::ExternalLinkage, name, m_module);

	for (auto i = 0u; i < arguments.size(); i++)
	{
		auto argument = arguments[i];
		auto name = argument->ID()->getText();

		locals[name] = function->getArg(i);
	}

	VisitorV1 child(m_context, m_module, m_block, function, locals);

	child.visit(context->block());

	verifyFunction(*function);

	return function;
}

antlrcpp::Any VisitorV1::visitTypeName(dzParser::TypeNameContext *context)
{
	auto typeName = context->ID()->getText();

	if (typeName == "int")
	{
		return (Type *)Type::getInt32Ty(m_context);
	}

	throw new UnknownTypeException(context, typeName);
}

antlrcpp::Any VisitorV1::visitBlock(dzParser::BlockContext *context)
{
	auto outer = BasicBlock::Create(m_context, "entry", m_function);

	VisitorV1 visitor(m_context, m_module, outer, m_function, m_locals);

	auto statements = context->statement();

	auto inner = populateBlock(visitor
		, begin(statements)
		, end(statements)
		);

	inner.visit(context->ret());

	return outer;
}

antlrcpp::Any VisitorV1::visitRet(dzParser::RetContext *context)
{
	if (!context->value)
	{
		return nullptr;
	}

	IRBuilder<> builder(m_block);

	auto value = visitAs<Value *>(context->value);

	return builder.CreateRet(value);
}

antlrcpp::Any VisitorV1::visitConstant(dzParser::ConstantContext *context)
{
	auto value = context->INT()->toString();
	auto constant = (Value *)ConstantInt::get(Type::getInt32Ty(m_context), value, 10);

	return constant;
}

antlrcpp::Any VisitorV1::visitMember(dzParser::MemberContext *context)
{
	auto name = context->getText();

	auto result = m_locals.find(name);

	if (result == m_locals.end())
	{
		throw new UndeclaredIdentifierException(context, name);
	}

	return result->second;
}

antlrcpp::Any VisitorV1::visitCall(dzParser::CallContext *context)
{
	IRBuilder<> builder(m_block);

	auto name = context->ID()->getText();
	auto target = m_module.getFunction(name);

	if (!target)
	{
		throw new UndeclaredIdentifierException(context, name);
	}

	std::vector<Value *> arguments;

	for (auto argument : context->expression())
	{
		auto value = visitAs<Value *>(argument);

		arguments.push_back(value);
	}

	return (Value *)builder.CreateCall(target, arguments);
}

antlrcpp::Any VisitorV1::visitBinary(dzParser::BinaryContext *context)
{
	IRBuilder<> builder(m_block);

	auto left = visitAs<Value *>(context->left);
	auto right = visitAs<Value *>(context->right);

	if (context->op->getText() == "+")
	{
		return builder.CreateAdd(left, right);
	}

	if (context->op->getText() == "-")
	{
		return builder.CreateSub(left, right);
	}

	if (context->op->getText() == "*")
	{
		return builder.CreateMul(left, right);
	}

	if (context->op->getText() == "/")
	{
		return builder.CreateSDiv(left, right);
	}

	if (context->op->getText() == "<")
	{
		return builder.CreateCmp(CmpInst::Predicate::ICMP_SLT, left, right);
	}

	if (context->op->getText() == "<=")
	{
		return builder.CreateCmp(CmpInst::Predicate::ICMP_SLE, left, right);
	}

	if (context->op->getText() == ">")
	{
		return builder.CreateCmp(CmpInst::Predicate::ICMP_SGT, left, right);
	}

	if (context->op->getText() == ">=")
	{
		return builder.CreateCmp(CmpInst::Predicate::ICMP_SGE, left, right);
	}

	if (context->op->getText() == "==")
	{
		return builder.CreateCmp(CmpInst::Predicate::ICMP_EQ, left, right);
	}

	if (context->op->getText() == "!=")
	{
		return builder.CreateCmp(CmpInst::Predicate::ICMP_NE, left, right);
	}

	return nullptr;
}

antlrcpp::Any VisitorV1::visitConditional(dzParser::ConditionalContext *context)
{
	IRBuilder<> builder(m_block);

	auto condition = visitAs<Value *>(context->expression());

	auto ifTrue = visitAs<BasicBlock *>(context->block());
	auto ifFalse = BasicBlock::Create(m_context, "ifFalse", m_function);

	builder.CreateCondBr(condition, ifTrue, ifFalse);

	return ifFalse;
}

antlrcpp::Any VisitorV1::visitStructure(dzParser::StructureContext *context)
{
	auto name = context->ID()->getText();

	std::vector<Type *> fieldTypes;

	for (auto field : context->field())
	{
		auto fieldType = visitAs<Type *>(field->typeName());

		fieldTypes.push_back(fieldType);
	}

	auto type = StructType::create(fieldTypes, name);

	return nullptr;
}

VisitorV1 VisitorV1::populateBlock(VisitorV1 visitor
	, std::vector<dzParser::StatementContext *>::const_iterator iterator
	, std::vector<dzParser::StatementContext *>::const_iterator end
	)
{
	if (iterator == end)
	{
		return visitor;
	}

	auto result = visitor.visit(*iterator);

	if (result.is<BasicBlock *>())
	{
		auto block = result.as<BasicBlock *>();

		VisitorV1 child(m_context, m_module, block, m_function, visitor.m_locals);

		return populateBlock(child, next(iterator), end);
	}

	if (result.is<LoadInst *>())
	{
		auto local = result.as<LoadInst *>();

		auto locals = visitor.m_locals;

		locals[local->getName().str()] = local;

		VisitorV1 child(m_context, m_module, visitor.m_block, m_function, locals);

		return populateBlock(child, next(iterator), end);
	}

	return populateBlock(visitor, next(iterator), end);
}
