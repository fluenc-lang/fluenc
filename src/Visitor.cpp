#include <numeric>

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Verifier.h>

#include "Visitor.h"
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
#include "Namespace.h"

#include "nodes/TerminatorNode.h"
#include "nodes/StringLiteralNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/StackSegmentNode.h"
#include "nodes/MemberAccessNode.h"
#include "nodes/LocalNode.h"
#include "nodes/IntegralLiteralNode.h"
#include "nodes/FunctionCallProxyNode.h"
#include "nodes/ExportedFunctionTerminatorNode.h"
#include "nodes/ExpansionNode.h"
#include "nodes/BlockInstructionNode.h"
#include "nodes/ConditionalNode.h"
#include "nodes/ContinuationNode.h"
#include "nodes/CharacterLiteralNode.h"
#include "nodes/ImportedFunctionNode.h"
#include "nodes/InstantiationNode.h"
#include "nodes/FunctionCallNode.h"
#include "nodes/GlobalNode.h"
#include "nodes/LazySinkNode.h"
#include "nodes/ReferenceSinkNode.h"
#include "nodes/LazyEvaluationNode.h"
#include "nodes/JunctionNode.h"
#include "nodes/IndexSinkNode.h"
#include "nodes/EmptyArrayNode.h"
#include "nodes/FunctionNode.h"
#include "nodes/ExportedFunctionNode.h"
#include "nodes/BooleanLiteralNode.h"
#include "nodes/BlockStackFrameNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/ArraySinkNode.h"
#include "nodes/ArrayElementNode.h"

#include "types/Prototype.h"
#include "types/IteratorType.h"

#include "values/ScalarValue.h"
#include "values/ReferenceValue.h"
#include "values/IteratorValue.h"
#include "values/ArrayValue.h"
#include "values/IndexedValue.h"

Visitor::Visitor(const Type *iteratorType, Node *alpha, Node *beta)
	: m_iteratorType(iteratorType)
	, m_alpha(alpha)
	, m_beta(beta)
{
}

void populateInstructions(const std::vector<std::string> &namespaces
	, const std::vector<antlrcpp::Any> &instructions
	, std::unique_ptr<llvm::Module> *module
	, std::unique_ptr<llvm::LLVMContext> *context
	, std::vector<CallableNode *> &roots
	, std::multimap<std::string, CallableNode *> &functions
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

			if (instruction.is<CallableNode *>())
			{
				auto callable = instruction.as<CallableNode *>();

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
			else if (instruction.is<GlobalNode *>())
			{
				auto global = instruction.as<GlobalNode *>();

				qualifiedName << global->name();

				globals.insert({ qualifiedName.str(), global });
			}
		}
	}
}

antlrcpp::Any Visitor::visitProgram(dzParser::ProgramContext *context)
{
	auto llvmContext = std::make_unique<llvm::LLVMContext>();
	auto module = std::make_unique<llvm::Module>("dz", *llvmContext);

	std::vector<CallableNode *> roots;
	std::multimap<std::string, CallableNode *> functions;
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

antlrcpp::Any Visitor::visitFunction(dzParser::FunctionContext *context)
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
		auto import = new ImportedFunctionNode(context
			, name
			, arguments
			, visit<ITypeName *>(context->typeName())
			);

		return static_cast<CallableNode *>(import);
	}

	if (attribute == FunctionAttribute::Export)
	{
		auto terminator = new ExportedFunctionTerminatorNode();

		Visitor visitor(nullptr, terminator, nullptr);

		auto entryPoint = new ExportedFunctionNode(name
			, visitor.visit<Node *>(block)
			, visitor.visit<ITypeName *>(context->typeName())
			);

		return static_cast<CallableNode *>(entryPoint);
	}

	auto iteratorType = new IteratorType();

	auto terminator = new TerminatorNode(name, attribute);

	Visitor visitor(iteratorType, terminator, nullptr);

	auto content = visitor.visit<Node *, BlockInstructionNode *>(block);

	if (content->containsIterator())
	{
		attribute = FunctionAttribute::Iterator;
	}

	auto function = new FunctionNode(attribute
		, name
		, arguments
		, content
		);

	return static_cast<CallableNode *>(function);
}

antlrcpp::Any Visitor::visitRegularType(dzParser::RegularTypeContext *context)
{
	return static_cast<ITypeName *>(
		new DzTypeName(context
			, context->getText()
		)
	);
}

antlrcpp::Any Visitor::visitFunctionType(dzParser::FunctionTypeContext *context)
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

antlrcpp::Any Visitor::visitStandardArgument(dzParser::StandardArgumentContext *context)
{
	auto argument = new DzArgument(context->ID()->getText()
		, visit<ITypeName *>(context->typeName())
		);

	return static_cast<DzBaseArgument *>(argument);
}

antlrcpp::Any Visitor::visitTupleArgument(dzParser::TupleArgumentContext *context)
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

antlrcpp::Any Visitor::visitRet(dzParser::RetContext *context)
{
	if (context->chained)
	{
		auto continuation = visit<Node *>(context->chained);

		auto ret = new ReturnNode(m_iteratorType, m_alpha, continuation);

		Visitor visitor(nullptr, ret, nullptr);

		auto value = visitor
			.visit<Node *>(context->value);

		auto instruction = new BlockInstructionNode(value, true);

		return static_cast<Node *>(instruction);
	}

	auto ret = new ReturnNode(m_iteratorType, m_alpha, nullptr);

	Visitor visitor(nullptr, ret, nullptr);

	auto value = visitor
		.visit<Node *>(context->value);

	auto instruction = new BlockInstructionNode(value, false);

	return static_cast<Node *>(instruction);
}

antlrcpp::Any Visitor::visitBlock(dzParser::BlockContext *context)
{
	auto expressions = context->expression();

	auto ret = visit<Node *, BlockInstructionNode *>(context->ret());

	auto result = std::accumulate(rbegin(expressions), rend(expressions), ret, [this](BlockInstructionNode *consumer, dzParser::ExpressionContext *expression)
	{
		auto stackFrame = new BlockStackFrameNode(consumer);

		Visitor visitor(m_iteratorType, stackFrame, m_alpha);

		auto value = visitor
			.visit<Node *>(expression);

		if (auto instruction = dynamic_cast<const BlockInstructionNode *>(value))
		{
			return new BlockInstructionNode(instruction
				, instruction->containsIterator() || consumer->containsIterator()
				);
		}

		return new BlockInstructionNode(value
			, consumer->containsIterator()
			);
	});

	return static_cast<Node *>(result);
}

antlrcpp::Any Visitor::visitBinary(dzParser::BinaryContext *context)
{
	auto binary = new BinaryNode(m_alpha
		, context->OP()->getText()
		);

	Visitor leftVisitor(m_iteratorType, binary, nullptr);

	auto left = leftVisitor
		.visit<Node *>(context->left);

	Visitor rightVisitor(m_iteratorType, left, nullptr);

	auto right = rightVisitor
		.visit<Node *>(context->right);

	return right;
}

antlrcpp::Any Visitor::visitCall(dzParser::CallContext *context)
{
	auto expression = context->expression();
	auto name = context->ID()->getText();

	auto call = new FunctionCallNode(context, name);

	std::vector<Node *> values;

	std::transform(begin(expression), end(expression), std::back_insert_iterator(values), [this](dzParser::ExpressionContext *parameter)
	{
		auto sink = new ReferenceSinkNode(TerminatorNode::instance());

		Visitor visitor(m_iteratorType, sink, nullptr);

		return visitor
			.visit<Node *>(parameter);
	});

	auto evaluation = new LazyEvaluationNode(call);
	auto segment = new StackSegmentNode(values, evaluation, TerminatorNode::instance());
	auto proxy = new FunctionCallProxyNode(name, m_alpha, segment);

	return static_cast<Node *>(proxy);
}

antlrcpp::Any Visitor::visitWith(dzParser::WithContext *context)
{
	auto assignments = context->assignment();

	std::vector<std::string> fields;

	std::transform(begin(assignments), end(assignments), std::back_insert_iterator(fields), [](dzParser::AssignmentContext *assignment)
	{
		return assignment->field()->ID()->getText();
	});

	auto instantiation = new InstantiationNode(m_alpha
		, WithPrototypeProvider::instance()
		, fields
		);

	return std::accumulate(begin(assignments), end(assignments), static_cast<Node *>(instantiation), [this](auto consumer, dzParser::AssignmentContext *assignment)
	{
		Visitor visitor(m_iteratorType, consumer, nullptr);

		return visitor
			.visit<Node *>(assignment->expression());
	});
}

antlrcpp::Any Visitor::visitMember(dzParser::MemberContext *context)
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
		auto member = new MemberAccessNode(context, visit<Node *>(with), path);

		return static_cast<Node *>(member);
	}

	auto member = new MemberAccessNode(context, m_alpha, path);

	return static_cast<Node *>(member);
}

antlrcpp::Any Visitor::visitInt32Literal(dzParser::Int32LiteralContext *context)
{
	auto constant = new IntegralLiteralNode(m_alpha
		, DzTypeName::int32()
		, context->INT()->getText()
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any Visitor::visitInt64Literal(dzParser::Int64LiteralContext *context)
{
	auto constant = new IntegralLiteralNode(m_alpha
		, DzTypeName::int64()
		, context->INT()->getText()
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any Visitor::visitBoolLiteral(dzParser::BoolLiteralContext *context)
{
	auto constant = new BooleanLiteralNode(m_alpha
		, context->BOOL()->getText()
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any Visitor::visitStringLiteral(dzParser::StringLiteralContext *context)
{
	auto constant = new StringLiteralNode(m_alpha
		, context->STRING()->getText()
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any Visitor::visitUint32Literal(dzParser::Uint32LiteralContext *context)
{
	auto constant = new IntegralLiteralNode(m_alpha
		, DzTypeName::uint32()
		, context->INT()->getText()
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any Visitor::visitStructure(dzParser::StructureContext *context)
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
			Visitor visitor(m_iteratorType, TerminatorNode::instance(), nullptr);

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

antlrcpp::Any Visitor::visitInstantiation(dzParser::InstantiationContext *context)
{
	auto assignments = context->assignment();

	std::vector<std::string> fields;

	std::transform(begin(assignments), end(assignments), std::back_insert_iterator(fields), [](dzParser::AssignmentContext *assignment)
	{
		return assignment->field()->ID()->getText();
	});

	auto typeName = visit<ITypeName *>(context->typeName());

	auto prototypeProvider = new DefaultPrototypeProvider(typeName);
	auto instantiation = new InstantiationNode(m_alpha
		, prototypeProvider
		, fields
		);

	return std::accumulate(begin(assignments), end(assignments), static_cast<Node *>(instantiation), [this](auto consumer, dzParser::AssignmentContext *assignment)
	{
		Visitor visitor(m_iteratorType, consumer, nullptr);

		return visitor
			.visit<Node *>(assignment->expression());
	});
}

antlrcpp::Any Visitor::visitConditional(dzParser::ConditionalContext *context)
{
	Visitor blockVisitor(m_iteratorType, m_beta, nullptr);

	auto block = blockVisitor
		.visit<Node *, BlockInstructionNode *>(context->block());

	auto conditional = new ConditionalNode(m_alpha, block);

	Visitor expressionVisitor(m_iteratorType, conditional, nullptr);

	auto condition = expressionVisitor
		.visit<Node *>(context->expression());

	auto instruction = new BlockInstructionNode(condition
		, block->containsIterator()
		);

	return static_cast<Node *>(instruction);
}

antlrcpp::Any Visitor::visitGlobal(dzParser::GlobalContext *context)
{
	auto name = context->ID()->getText();

	Visitor visitor(m_iteratorType, TerminatorNode::instance(), nullptr);

	auto literal = visitor
		.visit<Node *>(context->expression());

	return new GlobalNode(literal, name);
}

antlrcpp::Any Visitor::visitNothing(dzParser::NothingContext *context)
{
	UNUSED(context);

	auto constant = new IntegralLiteralNode(m_alpha
		, DzTypeName::without()
		, "0"
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any Visitor::visitGroup(dzParser::GroupContext *context)
{
	return visit<Node *>(context->expression());
}

antlrcpp::Any Visitor::visitExpansion(dzParser::ExpansionContext *context)
{
	auto expansion = new ExpansionNode(m_alpha);

	Visitor visitor(m_iteratorType, expansion, nullptr);

	return visitor
		.visit<Node *>(context->expression());
}

antlrcpp::Any Visitor::visitContinuation(dzParser::ContinuationContext *context)
{
	auto continuation = new ContinuationNode();

	auto expressions = context->expression();

	return std::accumulate(begin(expressions), end(expressions), static_cast<Node *>(continuation), [this](Node *consumer, dzParser::ExpressionContext *parameter)
	{
		Visitor visitor(m_iteratorType, consumer, nullptr);

		auto result = visitor
			.visit<Node *>(parameter);

		return result;
	});
}

antlrcpp::Any Visitor::visitArray(dzParser::ArrayContext *context)
{
	auto expressions = context->expression();

	if (expressions.empty())
	{
		auto empty = new EmptyArrayNode(TerminatorNode::instance());

		return static_cast<Node *>(empty);
	}

	std::vector<Indexed<dzParser::ExpressionContext *>> indexed;

	std::transform(begin(expressions), end(expressions), index_iterator(0u), std::back_insert_iterator(indexed), [=](auto x, auto y) -> Indexed<dzParser::ExpressionContext *>
	{
		return { y, x };
	});

	auto firstValue = std::accumulate(begin(indexed), end(indexed), (Node *)TerminatorNode::instance(), [&](auto next, Indexed<dzParser::ExpressionContext *> expression)
	{
		auto indexSink = new IndexSinkNode(expression.index, next);
		auto referenceSink = new ReferenceSinkNode(indexSink);

		Visitor visitor(m_iteratorType, referenceSink, nullptr);

		return visitor
			.visit<Node *>(expression.value);
	});

//	auto taintedSink = new TaintedSink(firstValue);
	auto lazySink = new ArraySinkNode(expressions.size(), m_alpha, firstValue);

	return static_cast<Node *>(lazySink);
}

antlrcpp::Any Visitor::visitCharLiteral(dzParser::CharLiteralContext *context)
{
	auto value = new CharacterLiteralNode(m_alpha
		, context->CHARACTER()->getText()
		);

	return static_cast<Node *>(value);
}

antlrcpp::Any Visitor::visitByteLiteral(dzParser::ByteLiteralContext *context)
{
	auto constant = new IntegralLiteralNode(m_alpha
		, DzTypeName::byte()
		, context->INT()->getText()
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any Visitor::visitLocal(dzParser::LocalContext *context)
{
	auto local = new LocalNode(m_alpha
		, context->ID()->getText()
		);

	Visitor visitor(m_iteratorType, local, nullptr);

	return visitor.visit<Node *>(context->expression());
}

antlrcpp::Any Visitor::visitInstruction(dzParser::InstructionContext *context)
{
	return visitChildren(context);
}

antlrcpp::Any Visitor::visitNs(dzParser::NsContext *context)
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
