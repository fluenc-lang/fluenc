#include <numeric>

#include <llvm/IR/Instructions.h>
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
#include "DzGlobal.h"
#include "DzGlobalTerminator.h"
#include "IndexIterator.h"
#include "DefaultPrototypeProvider.h"
#include "WithPrototypeProvider.h"
#include "StackSegment.h"

#include "types/Prototype.h"

VisitorV4::VisitorV4(DzValue *alpha, DzValue *beta)
	: m_alpha(alpha)
	, m_beta(beta)
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

	Stack values;

	EntryPoint entryPoint(nullptr
		, nullptr
		, nullptr
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

	for (auto global : context->global())
	{
		auto result = visit(global)
			.as<DzGlobal *>();

		for (auto &[_, globalValues] : result->build(entryPoint, values))
		{
			locals.insert({ result->name(), *globalValues.begin() });
		}
	}

	for (auto root : roots)
	{
		auto ep = entryPoint
			.withLocals(locals);

		root->build(ep, values);
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

		if (attribute == "recursive")
		{
			return FunctionAttribute::Recursive;
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

		VisitorV4 visitor(terminator, nullptr);

		auto entryPoint = new DzExportedFunction(name
			, visitor.visit(block)
			, visitor.visit(context->typeName())
			);

		return static_cast<DzCallable *>(entryPoint);
	}

	auto terminator = new DzTerminator(name);

	VisitorV4 visitor(terminator, nullptr);

	auto function = new DzFunction(attribute
		, name
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
	auto ret = new DzReturn(m_alpha, nullptr);

	VisitorV4 visitor(ret, nullptr);

	return visitor
		.visit(context->value);
}

antlrcpp::Any VisitorV4::visitBlock(dzParser::BlockContext *context)
{
	auto expressions = context->expression();

	auto result = std::accumulate(rbegin(expressions), rend(expressions), visit(context->ret()), [this](DzValue *consumer, dzParser::ExpressionContext *expression)
	{
		VisitorV4 visitor(consumer, m_alpha);

		return visitor
			.visit(expression)
			.as<DzValue *>();
	});

	return static_cast<DzValue *>(result);
}

antlrcpp::Any VisitorV4::visitBinary(dzParser::BinaryContext *context)
{
	auto binary = new DzBinary(m_alpha
		, context->OP()->getText()
		);

	VisitorV4 leftVisitor(binary, nullptr);

	auto left = leftVisitor
		.visit(context->left)
		.as<DzValue *>();

	VisitorV4 rightVisitor(left, nullptr);

	auto right = rightVisitor.visit(context->right);

	return static_cast<DzValue *>(right);
}

antlrcpp::Any VisitorV4::visitCall(dzParser::CallContext *context)
{
	auto expression = context->expression();

	auto call = new DzFunctionCall(context->ID()->getText()
		, expression.size()
		);

	auto value = std::accumulate(begin(expression), end(expression), static_cast<DzValue *>(call), [](DzValue *consumer, dzParser::ExpressionContext *parameter)
	{
		VisitorV4 visitor(consumer, nullptr);

		auto result = visitor
			.visit(parameter)
			.as<DzValue *>();

		return result;
	});

	auto segment = new StackSegment(value, m_alpha);

	return static_cast<DzValue *>(segment);
}

antlrcpp::Any VisitorV4::visitWith(dzParser::WithContext *context)
{
	auto assignments = context->assignment();

	std::vector<std::string> fields;

	std::transform(begin(assignments), end(assignments), std::back_insert_iterator(fields), [](dzParser::AssignmentContext *assignment)
	{
		return assignment->field()->ID()->getText();
	});

	auto instantiation = new DzInstantiation(m_alpha
		, WithPrototypeProvider::instance()
		, fields
		);

	auto value = std::accumulate(begin(assignments), end(assignments), static_cast<DzValue *>(instantiation), [](auto consumer, dzParser::AssignmentContext *assignment)
	{
		VisitorV4 visitor(consumer, nullptr);

		return visitor
			.visit(assignment->expression())
			.as<DzValue *>();
	});

	return static_cast<DzValue *>(value);
}

antlrcpp::Any VisitorV4::visitMember(dzParser::MemberContext *context)
{
	auto access = context->ID();

	auto path = std::accumulate(begin(access) + 1, end(access), (*begin(access))->getText(), [](auto name, antlr4::tree::TerminalNode *node)
	{
		std::stringstream ss;
		ss << name << "." << node->getText();

		return ss.str();
	});

	auto with = context->with();

	if (with)
	{
		auto member = new DzMemberAccess(visit(with), path);

		return static_cast<DzValue *>(member);
	}

	auto member = new DzMemberAccess(m_alpha, path);

	return static_cast<DzValue *>(member);
}

antlrcpp::Any VisitorV4::visitInt32Literal(dzParser::Int32LiteralContext *context)
{
	auto constant = new DzIntegralLiteral(m_alpha
		, DzTypeName::int32()
		, context->INT()->getText()
		);

	return static_cast<DzValue *>(constant);
}

antlrcpp::Any VisitorV4::visitInt64Literal(dzParser::Int64LiteralContext *context)
{
	auto constant = new DzIntegralLiteral(m_alpha
		, DzTypeName::int64()
		, context->INT()->getText()
		);

	return static_cast<DzValue *>(constant);
}

antlrcpp::Any VisitorV4::visitBoolLiteral(dzParser::BoolLiteralContext *context)
{
	auto constant = new DzBooleanLiteral(m_alpha
		, context->BOOL()->getText()
		);

	return static_cast<DzValue *>(constant);
}

antlrcpp::Any VisitorV4::visitStringLiteral(dzParser::StringLiteralContext *context)
{
	auto constant = new DzStringLiteral(m_alpha
		, context->STRING()->getText()
		);

	return static_cast<DzValue *>(constant);
}

antlrcpp::Any VisitorV4::visitUint32Literal(dzParser::Uint32LiteralContext *context)
{
	auto constant = new DzIntegralLiteral(m_alpha
		, DzTypeName::uint32()
		, context->INT()->getText()
		);

	return static_cast<DzValue *>(constant);
}

antlrcpp::Any VisitorV4::visitStructure(dzParser::StructureContext *context)
{
	auto name = context->name->getText();
	auto inputFields = context->field();

	std::vector<PrototypeField> fields;

	std::transform(begin(inputFields), end(inputFields), std::back_insert_iterator(fields), [](dzParser::FieldContext *field) -> PrototypeField
	{
		auto name = field->ID()->getText();

		if (field->literal())
		{
			VisitorV4 visitor(DzTerminator::instance(), nullptr);

			auto defaultValue = visitor
				.visit(field->literal())
				.as<DzValue *>();

			return { name, defaultValue };
		}

		return { name, nullptr };
	});

	std::vector<DzTypeName *> parentTypes;

	auto parentTypeNames = context->typeName();

	std::transform(begin(parentTypeNames), end(parentTypeNames), std::back_insert_iterator(parentTypes), [this](dzParser::TypeNameContext *typeName) -> DzTypeName *
	{
		return visit(typeName);
	});

	return new Prototype(name, fields, parentTypes);
}

antlrcpp::Any VisitorV4::visitInstantiation(dzParser::InstantiationContext *context)
{
	auto assignments = context->assignment();

	std::vector<std::string> fields;

	std::transform(begin(assignments), end(assignments), std::back_insert_iterator(fields), [](dzParser::AssignmentContext *assignment)
	{
		return assignment->field()->ID()->getText();
	});

	auto typeName = visit(context->typeName())
		.as<DzTypeName *>();

	auto prototypeProvider = new DefaultPrototypeProvider(typeName);
	auto instantiation = new DzInstantiation(m_alpha
		, prototypeProvider
		, fields
		);

	auto value = std::accumulate(begin(assignments), end(assignments), static_cast<DzValue *>(instantiation), [](auto consumer, dzParser::AssignmentContext *assignment)
	{
		VisitorV4 visitor(consumer, nullptr);

		return visitor
			.visit(assignment->expression())
			.as<DzValue *>();
	});

	return static_cast<DzValue *>(value);
}

antlrcpp::Any VisitorV4::visitConditional(dzParser::ConditionalContext *context)
{
	VisitorV4 blockVisitor(m_beta, nullptr);

	auto block = blockVisitor
		.visit(context->block())
		.as<DzValue *>();

	auto conditional = new DzConditional(m_alpha, block);

	VisitorV4 expressionVisitor(conditional, nullptr);

	auto condition = expressionVisitor
		.visit(context->expression())
		.as<DzValue *>();

	return condition;
}

antlrcpp::Any VisitorV4::visitGlobal(dzParser::GlobalContext *context)
{
	auto name = context->ID()->getText();

	auto terminator = new DzGlobalTerminator(name);

	VisitorV4 visitor(terminator, nullptr);

	auto literal = visitor
		.visit(context->literal())
		.as<DzValue *>();

	return new DzGlobal(literal, name);
}

antlrcpp::Any VisitorV4::visitNothing(dzParser::NothingContext *context)
{
	UNUSED(context);

	auto constant = new DzIntegralLiteral(m_alpha
		, DzTypeName::without()
		, "0"
		);

	return static_cast<DzValue *>(constant);
}

antlrcpp::Any VisitorV4::visitGroup(dzParser::GroupContext *context)
{
	return visit(context->expression());
}
