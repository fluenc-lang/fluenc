//#include "VisitorV1.h"

//VisitorV1::VisitorV1(LLVMContext &context, Module &module, BasicBlock *block, Function *function, std::map<std::string, Value *> locals)
//	: m_context(context)
//	, m_module(module)
//	, m_block(block)
//	, m_function(function)
//	, m_locals(locals)
//{
//}

//antlrcpp::Any VisitorV1::visitTypeName(dzParser::TypeNameContext *context)
//{
//	auto typeName = context->ID()->getText();

//	if (typeName == "int")
//	{
//		return (Type *)Type::getInt32Ty(m_context);
//	}

//	throw new UnknownTypeException(context, typeName);
//}

//antlrcpp::Any VisitorV1::visitMember(dzParser::MemberContext *context)
//{
//	auto name = context->getText();

//	auto result = m_locals.find(name);

//	if (result == m_locals.end())
//	{
//		throw new UndeclaredIdentifierException(context, name);
//	}

//	return result->second;
//}

//antlrcpp::Any VisitorV1::visitCall(dzParser::CallContext *context)
//{
//	IRBuilder<> builder(m_block);

//	auto name = context->ID()->getText();
//	auto target = m_module.getFunction(name);

//	if (!target)
//	{
//		throw new UndeclaredIdentifierException(context, name);
//	}

//	std::vector<Value *> arguments;

//	for (auto argument : context->expression())
//	{
//		auto value = visitAs<Value *>(argument);

//		arguments.push_back(value);
//	}

//	return (Value *)builder.CreateCall(target, arguments);
//}

//antlrcpp::Any VisitorV1::visitConditional(dzParser::ConditionalContext *context)
//{
//	IRBuilder<> builder(m_block);

//	auto condition = visitAs<Value *>(context->expression());

//	auto ifTrue = visitAs<BasicBlock *>(context->block());
//	auto ifFalse = BasicBlock::Create(m_context, "ifFalse", m_function);

//	builder.CreateCondBr(condition, ifTrue, ifFalse);

//	return ifFalse;
//}
