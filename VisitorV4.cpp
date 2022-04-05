#include <numeric>

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Verifier.h>

#include "VisitorV4.h"
#include "EntryPoint.h"
#include "DzArgument.h"
#include "DzTypeName.h"
#include "IndexIterator.h"
#include "DefaultPrototypeProvider.h"
#include "WithPrototypeProvider.h"
#include "Indexed.h"
#include "DzTupleArgument.h"
#include "IRBuilderEx.h"
#include "FunctionTypeName.h"

#include "nodes/Terminator.h"
#include "nodes/StringLiteral.h"
#include "nodes/TaintedSink.h"
#include "nodes/Return.h"
#include "nodes/StackSegment.h"
#include "nodes/MemberAccess.h"
#include "nodes/Local.h"
#include "nodes/IntegralLiteral.h"
#include "nodes/FunctionCallProxy.h"
#include "nodes/ExportedFunctionTerminator.h"
#include "nodes/Expansion.h"
#include "nodes/BlockInstruction.h"
#include "nodes/Conditional.h"
#include "nodes/Continuation.h"
#include "nodes/CharacterLiteral.h"
#include "nodes/ImportedFunction.h"
#include "nodes/Instantiation.h"
#include "nodes/FunctionCall.h"
#include "nodes/Global.h"
#include "nodes/LazySink.h"
#include "nodes/ReferenceSink.h"
#include "nodes/LazyEvaluation.h"
#include "nodes/Junction.h"
#include "nodes/IndexSink.h"
#include "nodes/EmptyArray.h"
#include "nodes/Function.h"
#include "nodes/ExportedFunction.h"
#include "nodes/BooleanLiteral.h"
#include "nodes/BlockStackFrame.h"
#include "nodes/Binary.h"
#include "nodes/ArraySink.h"
#include "nodes/ArrayElement.h"
#include "nodes/Namespace.h"

#include "types/Prototype.h"
#include "types/IteratorType.h"

#include "values/TypedValue.h"
#include "values/ReferenceValue.h"
#include "values/IteratorValue.h"
#include "values/TaintedValue.h"
#include "values/ArrayValue.h"
#include "values/IndexedValue.h"

VisitorV4::VisitorV4(const Type *iteratorType, Node *alpha, Node *beta)
	: m_iteratorType(iteratorType)
	, m_alpha(alpha)
	, m_beta(beta)
{
}

void populateInstructions(const std::vector<std::string> &namespaces
	, const std::vector<antlrcpp::Any> &instructions
	, std::unique_ptr<llvm::Module> *module
	, std::unique_ptr<llvm::LLVMContext> *context
	, std::vector<Callable *> &roots
	, std::multimap<std::string, Callable *> &functions
	, std::map<std::string, const BaseValue *> &locals
	, std::map<std::string, const Node *> &globals
	, std::map<std::string, Prototype *> &types
	)
{
	for (auto &instruction : instructions)
	{
		if (instruction.is<Namespace *>())
		{
			auto _namespace = instruction.as<Namespace *>();

			std::vector<std::string> nestedNamespaces(namespaces);

			nestedNamespaces.push_back(_namespace->name());

			populateInstructions(nestedNamespaces
				, _namespace->children()
				, module
				, context
				, roots
				, functions
				, locals
				, globals
				, types
				);
		}
		else
		{
			std::ostringstream qualifiedName;

			std::copy(begin(namespaces), end(namespaces)
				, std::ostream_iterator<std::string>(qualifiedName, "::")
				);

			if (instruction.is<Callable *>())
			{
				auto callable = instruction.as<Callable *>();

				if (callable->attribute() == FunctionAttribute::Export)
				{
					roots.push_back(callable);
				}
				else
				{
					qualifiedName << callable->name();

					functions.insert({ qualifiedName.str(), callable });
				}
			}
			else if (instruction.is<Prototype *>())
			{
				auto prototype = instruction.as<Prototype *>();

				qualifiedName << prototype->name();

				types.insert({ qualifiedName.str(), prototype });
			}
			else if (instruction.is<Global *>())
			{
				auto global = instruction.as<Global *>();

				qualifiedName << global->name();

				globals.insert({ qualifiedName.str(), global });
			}
		}
	}
}

antlrcpp::Any VisitorV4::visitProgram(dzParser::ProgramContext *context)
{
	auto llvmContext = std::make_unique<llvm::LLVMContext>();
	auto module = std::make_unique<llvm::Module>("dz", *llvmContext);

	std::vector<Callable *> roots;
	std::multimap<std::string, Callable *> functions;
	std::map<std::string, const BaseValue *> locals;
	std::map<std::string, const Node *> globals;
	std::map<std::string, Prototype *> types;

	auto instructions = context->instruction();

	std::vector<antlrcpp::Any> results;

	std::transform(begin(instructions), end(instructions), std::back_inserter(results), [this](auto instruction)
	{
		return dzBaseVisitor::visit(instruction);
	});

	populateInstructions(std::vector<std::string>()
		, results
		, &module
		, &llvmContext
		, roots
		, functions
		, locals
		, globals
		, types
		);

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
		, globals
		, types
		, values
		, nullptr
		);

	for (auto root : roots)
	{
		root->build(entryPoint, values);
	}

	module->print(llvm::errs(), nullptr);

	if (verifyModule(*module, &llvm::errs()))
	{
		throw new std::exception();
	}

	return new ModuleInfo(entryPoint
		, std::move(module)
		, std::move(llvmContext)
		);
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
		auto import = new ImportedFunction(name
			, arguments
			, visit<ITypeName *>(context->typeName())
			);

		return static_cast<Callable *>(import);
	}

	if (attribute == FunctionAttribute::Export)
	{
		auto terminator = new ExportedFunctionTerminator();

		VisitorV4 visitor(nullptr, terminator, nullptr);

		auto entryPoint = new ExportedFunction(name
			, visitor.visit<Node *>(block)
			, visitor.visit<ITypeName *>(context->typeName())
			);

		return static_cast<Callable *>(entryPoint);
	}

	auto iteratorType = new IteratorType();

	auto terminator = new Terminator(name, attribute);

	VisitorV4 visitor(iteratorType, terminator, nullptr);

	auto content = visitor.visit<Node *, BlockInstruction *>(block);

	if (content->containsIterator())
	{
		attribute = FunctionAttribute::Iterator;
	}

	auto function = new Function(attribute
		, name
		, arguments
		, content
		);

	return static_cast<Callable *>(function);
}

antlrcpp::Any VisitorV4::visitRegularType(dzParser::RegularTypeContext *context)
{
	return static_cast<ITypeName *>(
		new DzTypeName(context->getText())
		);
}

antlrcpp::Any VisitorV4::visitFunctionType(dzParser::FunctionTypeContext *context)
{
	auto parameterTypes = context->typeName();

	std::vector<const ITypeName *> types;

	std::transform(begin(parameterTypes), end(parameterTypes), std::back_inserter(types), [this](auto typeName)
	{
		return visit<ITypeName *>(typeName);
	});

	return static_cast<ITypeName *>(
		new FunctionTypeName(types)
		);
}

antlrcpp::Any VisitorV4::visitStandardArgument(dzParser::StandardArgumentContext *context)
{
	auto argument = new DzArgument(context->ID()->getText()
		, visit<ITypeName *>(context->typeName())
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
		auto continuation = visit<Node *>(context->chained);

		auto ret = new Return(m_iteratorType, m_alpha, continuation);

		VisitorV4 visitor(nullptr, ret, nullptr);

		auto value = visitor
			.visit<Node *>(context->value);

		auto instruction = new BlockInstruction(value, true);

		return static_cast<Node *>(instruction);
	}

	auto ret = new Return(m_iteratorType, m_alpha, nullptr);

	VisitorV4 visitor(nullptr, ret, nullptr);

	auto value = visitor
		.visit<Node *>(context->value);

	auto instruction = new BlockInstruction(value, false);

	return static_cast<Node *>(instruction);
}

antlrcpp::Any VisitorV4::visitBlock(dzParser::BlockContext *context)
{
	auto expressions = context->expression();

	auto ret = visit<Node *, BlockInstruction *>(context->ret());

	auto result = std::accumulate(rbegin(expressions), rend(expressions), ret, [this](BlockInstruction *consumer, dzParser::ExpressionContext *expression)
	{
		auto stackFrame = new BlockStackFrame(consumer);

		VisitorV4 visitor(m_iteratorType, stackFrame, m_alpha);

		auto value = visitor
			.visit<Node *>(expression);

		if (auto instruction = dynamic_cast<const BlockInstruction *>(value))
		{
			return new BlockInstruction(instruction
				, instruction->containsIterator() || consumer->containsIterator()
				);
		}

		return new BlockInstruction(value
			, consumer->containsIterator()
			);
	});

	return static_cast<Node *>(result);
}

antlrcpp::Any VisitorV4::visitBinary(dzParser::BinaryContext *context)
{
	auto binary = new Binary(m_alpha
		, context->OP()->getText()
		);

	VisitorV4 leftVisitor(m_iteratorType, binary, nullptr);

	auto left = leftVisitor
		.visit<Node *>(context->left);

	VisitorV4 rightVisitor(m_iteratorType, left, nullptr);

	auto right = rightVisitor
		.visit<Node *>(context->right);

	return right;
}

antlrcpp::Any VisitorV4::visitCall(dzParser::CallContext *context)
{
	auto expression = context->expression();
	auto name = context->ID()->getText();

	auto call = new FunctionCall(context, name);

	std::vector<Node *> values;

	std::transform(begin(expression), end(expression), std::back_insert_iterator(values), [this](dzParser::ExpressionContext *parameter)
	{
		auto sink = new ReferenceSink(Terminator::instance());

		VisitorV4 visitor(m_iteratorType, sink, nullptr);

		return visitor
			.visit<Node *>(parameter);
	});

	auto evaluation = new LazyEvaluation(call);
	auto segment = new StackSegment(values, evaluation, Terminator::instance());
	auto proxy = new FunctionCallProxy(name, m_alpha, segment);

	return static_cast<Node *>(proxy);
}

antlrcpp::Any VisitorV4::visitWith(dzParser::WithContext *context)
{
	auto assignments = context->assignment();

	std::vector<std::string> fields;

	std::transform(begin(assignments), end(assignments), std::back_insert_iterator(fields), [](dzParser::AssignmentContext *assignment)
	{
		return assignment->field()->ID()->getText();
	});

	auto instantiation = new Instantiation(m_alpha
		, WithPrototypeProvider::instance()
		, fields
		);

	return std::accumulate(begin(assignments), end(assignments), static_cast<Node *>(instantiation), [this](auto consumer, dzParser::AssignmentContext *assignment)
	{
		VisitorV4 visitor(m_iteratorType, consumer, nullptr);

		return visitor
			.visit<Node *>(assignment->expression());
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
		auto member = new MemberAccess(context, visit<Node *>(with), path);

		return static_cast<Node *>(member);
	}

	auto member = new MemberAccess(context, m_alpha, path);

	return static_cast<Node *>(member);
}

antlrcpp::Any VisitorV4::visitInt32Literal(dzParser::Int32LiteralContext *context)
{
	auto constant = new IntegralLiteral(m_alpha
		, DzTypeName::int32()
		, context->INT()->getText()
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any VisitorV4::visitInt64Literal(dzParser::Int64LiteralContext *context)
{
	auto constant = new IntegralLiteral(m_alpha
		, DzTypeName::int64()
		, context->INT()->getText()
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any VisitorV4::visitBoolLiteral(dzParser::BoolLiteralContext *context)
{
	auto constant = new BooleanLiteral(m_alpha
		, context->BOOL()->getText()
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any VisitorV4::visitStringLiteral(dzParser::StringLiteralContext *context)
{
	auto constant = new StringLiteral(m_alpha
		, context->STRING()->getText()
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any VisitorV4::visitUint32Literal(dzParser::Uint32LiteralContext *context)
{
	auto constant = new IntegralLiteral(m_alpha
		, DzTypeName::uint32()
		, context->INT()->getText()
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any VisitorV4::visitStructure(dzParser::StructureContext *context)
{
	auto name = context->name->getText();
	auto inputFields = context->field();

	std::vector<PrototypeFieldEmbryo> fields;

	std::transform(begin(inputFields), end(inputFields), std::back_insert_iterator(fields), [this](dzParser::FieldContext *field) -> PrototypeFieldEmbryo
	{
		auto name = field->ID()->getText();

		ITypeName *type = nullptr;

		if (field->typeName())
		{
			type = visit<ITypeName *>(field->typeName());
		}

		if (field->expression())
		{
			VisitorV4 visitor(m_iteratorType, Terminator::instance(), nullptr);

			auto defaultValue = visitor
				.visit<Node *>(field->expression());

			return { name, defaultValue, type };
		}

		return { name, nullptr, type };
	});

	std::vector<ITypeName *> parentTypes;

	auto parentTypeNames = context->typeName();

	std::transform(begin(parentTypeNames), end(parentTypeNames), std::back_insert_iterator(parentTypes), [this](dzParser::TypeNameContext *typeName) -> ITypeName *
	{
		return visit<ITypeName *>(typeName);
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

	auto typeName = visit<ITypeName *>(context->typeName());

	auto prototypeProvider = new DefaultPrototypeProvider(typeName);
	auto instantiation = new Instantiation(m_alpha
		, prototypeProvider
		, fields
		);

	return std::accumulate(begin(assignments), end(assignments), static_cast<Node *>(instantiation), [this](auto consumer, dzParser::AssignmentContext *assignment)
	{
		VisitorV4 visitor(m_iteratorType, consumer, nullptr);

		return visitor
			.visit<Node *>(assignment->expression());
	});
}

antlrcpp::Any VisitorV4::visitConditional(dzParser::ConditionalContext *context)
{
	VisitorV4 blockVisitor(m_iteratorType, m_beta, nullptr);

	auto block = blockVisitor
		.visit<Node *, BlockInstruction *>(context->block());

	auto conditional = new Conditional(m_alpha, block);

	VisitorV4 expressionVisitor(m_iteratorType, conditional, nullptr);

	auto condition = expressionVisitor
		.visit<Node *>(context->expression());

	auto instruction = new BlockInstruction(condition
		, block->containsIterator()
		);

	return static_cast<Node *>(instruction);
}

antlrcpp::Any VisitorV4::visitGlobal(dzParser::GlobalContext *context)
{
	auto name = context->ID()->getText();

	VisitorV4 visitor(m_iteratorType, Terminator::instance(), nullptr);

	auto literal = visitor
		.visit<Node *>(context->expression());

	return new Global(literal, name);
}

antlrcpp::Any VisitorV4::visitNothing(dzParser::NothingContext *context)
{
	UNUSED(context);

	auto constant = new IntegralLiteral(m_alpha
		, DzTypeName::without()
		, "0"
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any VisitorV4::visitGroup(dzParser::GroupContext *context)
{
	return visit<Node *>(context->expression());
}

antlrcpp::Any VisitorV4::visitExpansion(dzParser::ExpansionContext *context)
{
	auto expansion = new Expansion(m_alpha);

	VisitorV4 visitor(m_iteratorType, expansion, nullptr);

	return visitor
		.visit<Node *>(context->expression());
}

antlrcpp::Any VisitorV4::visitContinuation(dzParser::ContinuationContext *context)
{
	auto continuation = new Continuation();

	auto expressions = context->expression();

	return std::accumulate(begin(expressions), end(expressions), static_cast<Node *>(continuation), [this](Node *consumer, dzParser::ExpressionContext *parameter)
	{
		VisitorV4 visitor(m_iteratorType, consumer, nullptr);

		auto result = visitor
			.visit<Node *>(parameter);

		return result;
	});
}

antlrcpp::Any VisitorV4::visitArray(dzParser::ArrayContext *context)
{
	auto expressions = context->expression();

	std::vector<Indexed<dzParser::ExpressionContext *>> indexed;

	std::transform(begin(expressions), end(expressions), index_iterator(0u), std::back_insert_iterator(indexed), [=](auto x, auto y) -> Indexed<dzParser::ExpressionContext *>
	{
		return { y, x };
	});

	auto iteratorType = new IteratorType();

	auto firstElement = std::accumulate(begin(indexed), end(indexed), (Node *)nullptr, [&](auto next, auto)
	{
		return new ArrayElement(iteratorType, next);
	});

	if (!firstElement)
	{
		auto empty = new EmptyArray(Terminator::instance());

		return static_cast<Node *>(empty);
	}

	auto firstValue = std::accumulate(begin(indexed), end(indexed), (Node *)Terminator::instance(), [&](auto next, Indexed<dzParser::ExpressionContext *> expression)
	{
		auto k = new IndexSink(expression.index, next);
		auto sink = new ReferenceSink(k);

		VisitorV4 visitor(m_iteratorType, sink, nullptr);

		return visitor
			.visit<Node *>(expression.value);
	});

//	auto init = new DzArrayInit(firstElement, firstValue);

//	auto junction = new Junction(firstValue);
//	auto taintedSink = new TaintedSink(firstValue);
	auto lazySink = new ArraySink(iteratorType, m_alpha, firstElement, firstValue);

	return static_cast<Node *>(lazySink);
}

antlrcpp::Any VisitorV4::visitCharLiteral(dzParser::CharLiteralContext *context)
{
	auto value = new CharacterLiteral(m_alpha
		, context->CHARACTER()->getText()
		);

	return static_cast<Node *>(value);
}

antlrcpp::Any VisitorV4::visitByteLiteral(dzParser::ByteLiteralContext *context)
{
	auto constant = new IntegralLiteral(m_alpha
		, DzTypeName::byte()
		, context->INT()->getText()
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any VisitorV4::visitLocal(dzParser::LocalContext *context)
{
	auto local = new Local(m_alpha
		, context->ID()->getText()
		);

	VisitorV4 visitor(m_iteratorType, local, nullptr);

	return visitor.visit<Node *>(context->expression());
}

antlrcpp::Any VisitorV4::visitInstruction(dzParser::InstructionContext *context)
{
	return visitChildren(context);
}

antlrcpp::Any VisitorV4::visitNs(dzParser::NsContext *context)
{
	auto instructions = context->instruction();

	std::vector<antlrcpp::Any> children;

	std::transform(begin(instructions), end(instructions), std::back_inserter(children), [this ](auto instruction)
	{
		return dzBaseVisitor::visit(instruction);
	});

	return new Namespace(children
		, context->ID()->getText()
		);
}
