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
#include "DzInstantiation.h"
#include "DzGlobal.h"
#include "DzGlobalTerminator.h"
#include "IndexIterator.h"
#include "DefaultPrototypeProvider.h"
#include "WithPrototypeProvider.h"
#include "StackSegment.h"
#include "DzContinuation.h"
#include "DzExpansion.h"
#include "Indexed.h"
#include "DzArrayElement.h"
#include "DzArrayInit.h"
#include "BlockStackFrame.h"
#include "DzTupleArgument.h"
#include "DzEmptyArray.h"
#include "DzCharacterLiteral.h"
#include "LazyEvaluation.h"
#include "LazySink.h"
#include "TaintedSink.h"
#include "Junction.h"
#include "DzBlockInstruction.h"
#include "DzIteratorFunction.h"
#include "IRBuilderEx.h"
#include "ArraySink.h"

#include "types/Prototype.h"
#include "types/IteratorType.h"

#include "values/TypedValue.h"
#include "values/ReferenceValue.h"
#include "values/LazyValue.h"
#include "values/TaintedValue.h"
#include "values/ArrayValue.h"

VisitorV4::VisitorV4(const Type *iteratorType, DzValue *alpha, DzValue *beta)
	: m_iteratorType(iteratorType)
	, m_alpha(alpha)
	, m_beta(beta)
{
}

antlrcpp::Any VisitorV4::visitProgram(dzParser::ProgramContext *context)
{
	auto llvmContext = std::make_unique<llvm::LLVMContext>();
	auto module = std::make_unique<llvm::Module>("dz", *llvmContext);

	std::vector<DzCallable *> roots;
	std::multimap<std::string, DzCallable *> functions;
	std::map<std::string, const BaseValue *> locals;
	std::map<std::string, Prototype *> types;

	for (auto function : context->function())
	{
		auto result = visit<DzCallable *>(function);

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
		auto result = visit<Prototype *>(structure);

		types.insert({ result->name(), result });
	}

	Stack values;

	EntryPoint entryPoint(0
		, nullptr
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
		auto result = visit<DzGlobal *>(global);

		for (auto &[_, globalValues] : result->build(entryPoint, values))
		{
			locals.insert({ result->name(), globalValues.require<ReferenceValue>() });
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
	std::vector<DzBaseArgument *> arguments;

	for (auto argument : context->argument())
	{
		arguments.push_back(visit<DzBaseArgument *>(argument));
	}

	auto name = context->name->getText();
	auto block = context->block();

	auto attribute = getAttribute(context);

	if (attribute == FunctionAttribute::Import)
	{
		auto import = new DzImportedFunction(name
			, arguments
			, visit<DzTypeName *>(context->typeName())
			);

		return static_cast<DzCallable *>(import);
	}

	if (attribute == FunctionAttribute::Export)
	{
		auto terminator = new DzExportedFunctionTerminator();

		VisitorV4 visitor(nullptr, terminator, nullptr);

		auto entryPoint = new DzExportedFunction(name
			, visitor.visit<DzValue *>(block)
			, visitor.visit<DzTypeName *>(context->typeName())
			);

		return static_cast<DzCallable *>(entryPoint);
	}

	auto iteratorType = new IteratorType();

	auto terminator = new DzTerminator(name, attribute);

	VisitorV4 visitor(iteratorType, terminator, nullptr);

	auto content = visitor.visit<DzValue *, DzBlockInstruction *>(block);

	if (content->containsIterator())
	{
		attribute = FunctionAttribute::Iterator;
	}

	auto function = new DzFunction(attribute
		, name
		, arguments
		, content
		);

//	if (attribute == FunctionAttribute::Iterator)
//	{
//		auto iteratorFunction = new DzIteratorFunction(arguments
//			, iteratorType
//			, function
//			);

//		return static_cast<DzCallable *>(iteratorFunction);
//	}

	return static_cast<DzCallable *>(function);
}

antlrcpp::Any VisitorV4::visitTypeName(dzParser::TypeNameContext *context)
{
	return new DzTypeName(context->getText());
}

antlrcpp::Any VisitorV4::visitStandardArgument(dzParser::StandardArgumentContext *context)
{
	auto argument = new DzArgument(context->ID()->getText()
		, visit<DzTypeName *>(context->typeName())
		);

	return static_cast<DzBaseArgument *>(argument);
}

antlrcpp::Any VisitorV4::visitTupleArgument(dzParser::TupleArgumentContext *context)
{
	auto arguments = context->argument();

	std::vector<DzBaseArgument *> tupleArguments;

	std::transform(begin(arguments), end(arguments), std::back_insert_iterator(tupleArguments), [this](dzParser::ArgumentContext *argument)
	{
		return visit<DzBaseArgument *>(argument);
	});

	auto argument = new DzTupleArgument(tupleArguments);

	return static_cast<DzBaseArgument *>(argument);
}

antlrcpp::Any VisitorV4::visitRet(dzParser::RetContext *context)
{
	if (context->chained)
	{
		auto continuation = visit<DzValue *>(context->chained);

		auto ret = new DzReturn(m_iteratorType, m_alpha, continuation);

		VisitorV4 visitor(nullptr, ret, nullptr);

		auto value = visitor
			.visit<DzValue *>(context->value);

		auto instruction = new DzBlockInstruction(value, true);

		return static_cast<DzValue *>(instruction);
	}

	auto ret = new DzReturn(m_iteratorType, m_alpha, nullptr);

	VisitorV4 visitor(nullptr, ret, nullptr);

	auto value = visitor
		.visit<DzValue *>(context->value);

	auto instruction = new DzBlockInstruction(value, false);

	return static_cast<DzValue *>(instruction);
}

antlrcpp::Any VisitorV4::visitBlock(dzParser::BlockContext *context)
{
	auto expressions = context->expression();

	auto ret = visit<DzValue *, DzBlockInstruction *>(context->ret());

	auto result = std::accumulate(rbegin(expressions), rend(expressions), ret, [this](DzBlockInstruction *consumer, dzParser::ExpressionContext *expression)
	{
		auto stackFrame = new BlockStackFrame(consumer);

		VisitorV4 visitor(m_iteratorType, stackFrame, m_alpha);

		auto value = visitor
			.visit<DzValue *>(expression);

		if (auto instruction = dynamic_cast<const DzBlockInstruction *>(value))
		{
			return new DzBlockInstruction(instruction
				, instruction->containsIterator() || consumer->containsIterator()
				);
		}

		return new DzBlockInstruction(value
			, consumer->containsIterator()
			);
	});

	return static_cast<DzValue *>(result);
}

antlrcpp::Any VisitorV4::visitBinary(dzParser::BinaryContext *context)
{
	auto binary = new DzBinary(m_alpha
		, context->OP()->getText()
		);

	VisitorV4 leftVisitor(m_iteratorType, binary, nullptr);

	auto left = leftVisitor
		.visit<DzValue *>(context->left);

	VisitorV4 rightVisitor(m_iteratorType, left, nullptr);

	auto right = rightVisitor
		.visit<DzValue *>(context->right);

	return right;
}

antlrcpp::Any VisitorV4::visitCall(dzParser::CallContext *context)
{
	auto expression = context->expression();

	auto call = new DzFunctionCall(context->ID()->getText());

	std::vector<DzValue *> values;

	std::transform(begin(expression), end(expression), std::back_insert_iterator(values), [this](dzParser::ExpressionContext *parameter)
	{
		auto evaluation = new LazyEvaluation();

		VisitorV4 visitor(m_iteratorType, evaluation, nullptr);

		return visitor
			.visit<DzValue *>(parameter);
	});

	auto segment = new StackSegment(values, call, m_alpha);

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

	return std::accumulate(begin(assignments), end(assignments), static_cast<DzValue *>(instantiation), [this](auto consumer, dzParser::AssignmentContext *assignment)
	{
		VisitorV4 visitor(m_iteratorType, consumer, nullptr);

		return visitor
			.visit<DzValue *>(assignment->expression());
	});
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
		auto member = new DzMemberAccess(visit<DzValue *>(with), path);

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

	std::vector<PrototypeFieldEmbryo> fields;

	std::transform(begin(inputFields), end(inputFields), std::back_insert_iterator(fields), [this](dzParser::FieldContext *field) -> PrototypeFieldEmbryo
	{
		auto name = field->ID()->getText();

		DzTypeName *type = nullptr;

		if (field->typeName())
		{
			type = visit<DzTypeName *>(field->typeName());
		}

		if (field->expression())
		{
			VisitorV4 visitor(m_iteratorType, DzTerminator::instance(), nullptr);

			auto defaultValue = visitor
				.visit<DzValue *>(field->expression());

			return { name, defaultValue, type };
		}

		return { name, nullptr, type };
	});

	std::vector<DzTypeName *> parentTypes;

	auto parentTypeNames = context->typeName();

	std::transform(begin(parentTypeNames), end(parentTypeNames), std::back_insert_iterator(parentTypes), [this](dzParser::TypeNameContext *typeName) -> DzTypeName *
	{
		return visit<DzTypeName *>(typeName);
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

	auto typeName = visit<DzTypeName *>(context->typeName());

	auto prototypeProvider = new DefaultPrototypeProvider(typeName);
	auto instantiation = new DzInstantiation(m_alpha
		, prototypeProvider
		, fields
		);

	return std::accumulate(begin(assignments), end(assignments), static_cast<DzValue *>(instantiation), [this](auto consumer, dzParser::AssignmentContext *assignment)
	{
		VisitorV4 visitor(m_iteratorType, consumer, nullptr);

		return visitor
			.visit<DzValue *>(assignment->expression());
	});
}

antlrcpp::Any VisitorV4::visitConditional(dzParser::ConditionalContext *context)
{
	VisitorV4 blockVisitor(m_iteratorType, m_beta, nullptr);

	auto block = blockVisitor
		.visit<DzValue *, DzBlockInstruction *>(context->block());

	auto conditional = new DzConditional(m_alpha, block);

	VisitorV4 expressionVisitor(m_iteratorType, conditional, nullptr);

	auto condition = expressionVisitor
		.visit<DzValue *>(context->expression());

	auto instruction = new DzBlockInstruction(condition
		, block->containsIterator()
		);

	return static_cast<DzValue *>(instruction);
}

antlrcpp::Any VisitorV4::visitGlobal(dzParser::GlobalContext *context)
{
	auto name = context->ID()->getText();

	auto terminator = new DzGlobalTerminator(name);

	VisitorV4 visitor(m_iteratorType, terminator, nullptr);

	auto literal = visitor
		.visit<DzValue *>(context->literal());

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
	return visit<DzValue *>(context->expression());
}

antlrcpp::Any VisitorV4::visitExpansion(dzParser::ExpansionContext *context)
{
	auto expansion = new DzExpansion(m_alpha);

	VisitorV4 visitor(m_iteratorType, expansion, nullptr);

	return visitor
		.visit<DzValue *>(context->expression());
}

antlrcpp::Any VisitorV4::visitContinuation(dzParser::ContinuationContext *context)
{
	auto continuation = new DzContinuation();

	auto expressions = context->expression();

	return std::accumulate(begin(expressions), end(expressions), static_cast<DzValue *>(continuation), [this](DzValue *consumer, dzParser::ExpressionContext *parameter)
	{
		VisitorV4 visitor(m_iteratorType, consumer, nullptr);

		auto result = visitor
			.visit<DzValue *>(parameter);

		return result;
	});
}

antlrcpp::Any VisitorV4::visitArray(dzParser::ArrayContext *context)
{
	auto expressions = context->expression();

	std::vector<Indexed<dzParser::ExpressionContext *>> indexed;

	std::transform(rbegin(expressions), rend(expressions), index_iterator(1u), std::back_insert_iterator(indexed), [=](auto x, auto y) -> Indexed<dzParser::ExpressionContext *>
	{
		return { expressions.size() - y, x };
	});

	auto iteratorType = new IteratorType();

	auto firstElement = std::accumulate(begin(indexed), end(indexed), (DzValue *)nullptr, [&](auto next, Indexed<dzParser::ExpressionContext *> expression)
	{
		auto element = new DzArrayElement(iteratorType, expression.index, next);

		VisitorV4 visitor(m_iteratorType, element, nullptr);

		return visitor
			.visit<DzValue *>(expression.value);
	});

	if (firstElement)
	{
		auto init = new DzArrayInit(firstElement);
		auto junction = new Junction(init);
		auto taintedSink = new TaintedSink(junction);
		auto lazySink = new ArraySink(iteratorType, m_alpha, taintedSink);

		return static_cast<DzValue *>(lazySink);
	}

	auto empty = new DzEmptyArray(DzTerminator::instance());

	return static_cast<DzValue *>(empty);
}

antlrcpp::Any VisitorV4::visitCharLiteral(dzParser::CharLiteralContext *context)
{
	auto value = new DzCharacterLiteral(m_alpha
		, context->CHARACTER()->getText()
		);

	return static_cast<DzValue *>(value);
}

antlrcpp::Any VisitorV4::visitByteLiteral(dzParser::ByteLiteralContext *context)
{
	auto constant = new DzIntegralLiteral(m_alpha
		, DzTypeName::byte()
		, context->INT()->getText()
		);

	return static_cast<DzValue *>(constant);
}
