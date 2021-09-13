#include <numeric>

#include <llvm/IR/Verifier.h>

#include "VisitorV4.h"
#include "EntryPoint.h"
#include "DzFunction.h"
#include "DzExportedFunctionTerminator.h"
#include "DzExportedFunction.h"
#include "DzTerminator.h"
#include "DzIntegralLiteral.h"
#include "DzBinary.h"
#include "DzFunctionCall.h"
#include "DzMemberAccess.h"
#include "DzArgument.h"
#include "DzTypeName.h"
#include "DzConditional.h"
#include "DzReturn.h"
#include "DzBooleanLiteral.h"
#include "DzStringLiteral.h"
#include "DzImportedFunction.h"
#include "DzStruct.h"
#include "DzInstantiation.h"

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
	std::map<std::string, TypedValue> locals;
	std::map<std::string, Prototype *> types;

	for (auto function : context->function())
	{
		auto result = visit(function)
			.as<DzCallable *>();

		if (result->attribute() == FunctionAttribute::Export)
		{
			roots.push_back(result);
		}
		else
		{
			functions.insert({ result->name(), result });
		}
	}

	for (auto structure : context->structure())
	{
		auto result = visit(structure)
			.as<Prototype *>();

		types.insert({ result->tag(), result });
	}

	for (auto root : roots)
	{
		Stack values;

		EntryPoint entryPoint(nullptr
			, nullptr
			, nullptr
			, nullptr
			, &module
			, &llvmContext
			, "term"
			, functions
			, locals
			, types
			, values
			);

		root->build(entryPoint, values);
	}

	module->print(llvm::errs(), nullptr);

	if (verifyModule(*module, &llvm::errs()))
	{
		throw new std::exception();
	}

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
	std::vector<DzArgument *> arguments;

	for (auto argument : context->argument())
	{
		arguments.push_back(visit(argument));
	}

	auto name = context->name->getText();
	auto block = context->block();

	auto attribute = getAttribute(context);

	if (attribute == FunctionAttribute::Import)
	{
		auto import = new DzImportedFunction(name
			, arguments
			, visit(context->typeName())
			);

		return static_cast<DzCallable *>(import);
	}

	if (attribute == FunctionAttribute::Export)
	{
		auto terminator = new DzExportedFunctionTerminator();

		VisitorV4 visitor(terminator);

		auto entryPoint = new DzExportedFunction(name
			, visitor.visit(block)
			, visitor.visit(context->typeName())
			);

		return static_cast<DzCallable *>(entryPoint);
	}

	auto terminator = new DzTerminator(name);

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

antlrcpp::Any VisitorV4::visitRet(dzParser::RetContext *context)
{
	auto ret = new DzReturn(m_consumer);

	VisitorV4 visitor(ret);

	return visitor
		.visit(context->value);
}

antlrcpp::Any VisitorV4::visitBlock(dzParser::BlockContext *context)
{
	auto conditionals = context->conditional();

	auto result = std::accumulate(rbegin(conditionals), rend(conditionals), visit(context->ret()), [this](DzValue *ifFalse, dzParser::ConditionalContext *conditional)
	{
		auto value = new DzConditional(ifFalse
			, visit(conditional->block())
			, visit(conditional->expression())
			, m_consumer
			);

		return static_cast<DzValue *>(value);
	});

	return static_cast<DzValue *>(result);
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

	return static_cast<DzValue *>(right);
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
	auto access = context->ID();

	auto k = std::accumulate(begin(access) + 1, end(access), (*begin(access))->getText(), [](auto x, antlr4::tree::TerminalNode *y)
	{
		std::stringstream ss;
		ss << x << "." << y->getText();

		return ss.str();
	});

	auto member = new DzMemberAccess(m_consumer, k);

	return static_cast<DzValue *>(member);
}

antlrcpp::Any VisitorV4::visitInt32Literal(dzParser::Int32LiteralContext *context)
{
	auto constant = new DzIntegralLiteral(m_consumer
		, DzTypeName::int32()
		, context->INT()->getText()
		);

	return static_cast<DzValue *>(constant);
}

antlrcpp::Any VisitorV4::visitInt64Literal(dzParser::Int64LiteralContext *context)
{
	auto constant = new DzIntegralLiteral(m_consumer
		, DzTypeName::int64()
		, context->INT()->getText()
		);

	return static_cast<DzValue *>(constant);
}

antlrcpp::Any VisitorV4::visitBoolLiteral(dzParser::BoolLiteralContext *context)
{
	auto constant = new DzBooleanLiteral(m_consumer
		, context->BOOL()->getText()
		);

	return static_cast<DzValue *>(constant);
}

antlrcpp::Any VisitorV4::visitStringLiteral(dzParser::StringLiteralContext *context)
{
	auto constant = new DzStringLiteral(m_consumer
		, context->STRING()->getText()
		);

	return static_cast<DzValue *>(constant);
}

antlrcpp::Any VisitorV4::visitStructure(dzParser::StructureContext *context)
{
	auto name = context->ID()->getText();

	std::vector<std::string> fields;

	for (auto field : context->field())
	{
		auto name = field->ID()->getText();

		fields.push_back(name);
	}

	return new Prototype(name, fields);
}

antlrcpp::Any VisitorV4::visitInstantiation(dzParser::InstantiationContext *context)
{
	auto assignments = context->assignment();

	std::vector<std::string> fields;

	std::transform(begin(assignments), end(assignments), std::back_insert_iterator(fields), [](dzParser::AssignmentContext *assignment)
	{
		return assignment->field()->ID()->getText();
	});

	auto instantiation = new DzInstantiation(m_consumer
		, visit(context->typeName())
		, fields
		);

	auto value = std::accumulate(rbegin(assignments), rend(assignments), static_cast<DzValue *>(instantiation), [](auto consumer, dzParser::AssignmentContext *assignment)
	{
		VisitorV4 visitor(consumer);

		return visitor
			.visit(assignment->expression())
			.as<DzValue *>();
	});

	return static_cast<DzValue *>(value);
}
