#include <deque>
#include <numeric>
#include <ranges>

#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>

#include "VisitorV4.h"
#include "EntryPoint.h"
#include "DzFunction.h"
#include "DzExportedFunctionTerminator.h"
#include "DzExportedFunction.h"
#include "DzFunctionTerminator.h"
#include "DzConstant.h"
#include "DzBinary.h"
#include "DzFunctionCall.h"
#include "DzMemberAccess.h"
#include "DzArgument.h"
#include "DzTypeName.h"

VisitorV4::VisitorV4(DzValue *consumer)
	: m_consumer(consumer)
{
}

antlrcpp::Any VisitorV4::visitProgram(dzParser::ProgramContext *context)
{
	auto llvmContext = std::make_unique<llvm::LLVMContext>();
	auto module = std::make_unique<llvm::Module>("dz", *llvmContext);

	std::vector<DzCallable *> roots;
	std::multimap<std::string, DzCallable *> functions;
	std::map<std::string, llvm::Value *> locals;

	for (auto function : context->function())
	{
		auto result = visit(function)
			.as<DzCallable *>();

		if (result->attribute() == FunctionAttribute::Export)
		{
			roots.push_back(result);
		}

		if (result->attribute() == FunctionAttribute::None)
		{
			functions.insert({ result->name(), result });
		}
	}

	for (auto root : roots)
	{
		Stack values;

		EntryPoint entryPoint(nullptr, nullptr, module, llvmContext, functions, locals);

		root->build(entryPoint, values);
	}

	module->print(llvm::errs(), nullptr);

	return new ModuleInfo(std::move(module), std::move(llvmContext));
}

FunctionAttribute getAttribute(dzParser::FunctionContext *ctx)
{
	if (ctx->attribute)
	{
		auto attribute = ctx->attribute->getText();

		if (attribute == "import")
		{
			return FunctionAttribute::Import;
		}

		if (attribute == "export")
		{
			return FunctionAttribute::Export;
		}
	}

	return FunctionAttribute::None;
}

antlrcpp::Any VisitorV4::visitFunction(dzParser::FunctionContext *context)
{
	auto attribute = getAttribute(context);

	if (attribute == FunctionAttribute::Import)
	{
		return defaultResult();
	}

	auto name = context->name->getText();
	auto block = context->block();

	std::vector<DzArgument *> arguments;

	for (auto argument : context->argument())
	{
		arguments.push_back(visit(argument));
	}

	if (attribute == FunctionAttribute::Export)
	{
		auto terminator = new DzExportedFunctionTerminator();

		VisitorV4 visitor(terminator);

		auto entryPoint = new DzExportedFunction(name
			, visitor.visit(block)
			);

		return static_cast<DzCallable *>(entryPoint);
	}

	auto terminator = new DzFunctionTerminator(name);

	VisitorV4 visitor(terminator);

	auto function = new DzFunction(name
		, arguments
		, visitor.visit(block)
		);

	return static_cast<DzCallable *>(function);
}

antlrcpp::Any VisitorV4::visitTypeName(dzParser::TypeNameContext *context)
{
	return new DzTypeName(context->ID()->getText());
}

antlrcpp::Any VisitorV4::visitArgument(dzParser::ArgumentContext *context)
{
	return new DzArgument(context->ID()->getText()
		, visit(context->typeName())
		);
}

antlrcpp::Any VisitorV4::visitConstant(dzParser::ConstantContext *context)
{
	auto constant = new DzConstant(m_consumer
		, context->INT()->getText()
		);

	return static_cast<DzValue *>(constant);
}

antlrcpp::Any VisitorV4::visitRet(dzParser::RetContext *context)
{
	return visit(context->value);
}

//class DzConditional : public DzValue
//{
//	public:
//		Stack build(const EntryPoint &entryPoint, Stack values) const override
//		{
//			return values;
//		}
//};

//class DzBlock : public DzValue
//{
//	public:
//		DzBlock(const std::vector<DzConditional *> &conditionals
//			, DzValue *)
//			: m_conditionals(conditionals)
//		{
//		}

//		Stack build(const EntryPoint &entryPoint, Stack values) const override
//		{
//			return values;
//		}

//	private:
//		std::vector<DzConditional *> m_conditionals;
//};

antlrcpp::Any VisitorV4::visitBlock(dzParser::BlockContext *context)
{
//	std::vector<DzConditional *> conditionals;

//	auto co = context->conditional();

//	std::transform(begin(co), end(co), std::back_inserter(conditionals), [this](dzParser::ConditionalContext *c)
//	{
//		return visit(c)
//			.as<DzConditional *>();
//	});



	return visit(context->ret());
}

antlrcpp::Any VisitorV4::visitBinary(dzParser::BinaryContext *context)
{
	auto binary = new DzBinary(m_consumer
		, context->OP()->getText()
		);

	VisitorV4 leftVisitor(binary);

	auto left = leftVisitor
		.visit(context->left)
		.as<DzValue *>();

	VisitorV4 rightVisitor(left);

	auto right = rightVisitor.visit(context->right);

	return right;
}

antlrcpp::Any VisitorV4::visitCall(dzParser::CallContext *context)
{
	auto expression = context->expression();

	auto call = new DzFunctionCall(m_consumer
		, context->ID()->getText()
		, expression.size()
		);

	auto value = std::accumulate(begin(expression), end(expression), static_cast<DzValue *>(call), [](DzValue *consumer, dzParser::ExpressionContext *parameter)
	{
		VisitorV4 visitor(consumer);

		auto result = visitor
			.visit(parameter)
			.as<DzValue *>();

		return result;
	});

	return static_cast<DzValue *>(value);
}

antlrcpp::Any VisitorV4::visitMember(dzParser::MemberContext *context)
{
	auto member = new DzMemberAccess(m_consumer
		, context->ID()->getText()
		);

	return static_cast<DzValue *>(member);
}

antlrcpp::Any VisitorV4::visitConditional(dzParser::ConditionalContext *context)
{
	return defaultResult();
}
