#include <numeric>

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
#include "Use.h"
#include "ModuleInfo.h"

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

Visitor::Visitor(const std::vector<std::string> &namespaces
	, const Type *iteratorType
	, const Node *alpha
	, const Node *beta
	)
	: m_namespaces(namespaces)
	, m_iteratorType(iteratorType)
	, m_alpha(alpha)
	, m_beta(beta)
{
}

std::vector<std::string> qualifiedNames(const std::vector<std::string> &namespaces, const std::string &name)
{
	if (name.rfind("::") == 0)
	{
		return { name.substr(2) };
	}

	std::ostringstream qualifiedName;

	std::copy(begin(namespaces), end(namespaces)
		, std::ostream_iterator<std::string>(qualifiedName, "::")
		);

	qualifiedName << name;

	return { qualifiedName.str(), name };
}

void populateInstructions(const std::vector<std::string> &namespaces
	, const std::vector<antlrcpp::Any> &instructions
	, std::vector<CallableNode *> &roots
	, std::multimap<std::string, CallableNode *> &functions
	, std::map<std::string, const BaseValue *> &locals
	, std::map<std::string, const Node *> &globals
	, std::map<std::string, Prototype *> &types
	, std::unordered_set<std::string> &uses
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
				, roots
				, functions
				, locals
				, globals
				, types
				, uses
				);
		}
		else
		{
			if (instruction.is<CallableNode *>())
			{
				auto callable = instruction.as<CallableNode *>();

				if (callable->attribute() == FunctionAttribute::Export)
				{
					roots.push_back(callable);
				}
				else
				{
					auto name = qualifiedNames(namespaces
						, callable->name()
						);

					functions.insert({ name[0], callable });
				}
			}
			else if (instruction.is<Prototype *>())
			{
				auto prototype = instruction.as<Prototype *>();

				auto name = qualifiedNames(namespaces
					, prototype->name()
					);

				types.insert({ name[0], prototype });
			}
			else if (instruction.is<GlobalNode *>())
			{
				auto global = instruction.as<GlobalNode *>();

				auto name = qualifiedNames(namespaces
					, global->name()
					);

				globals.insert({ name[0], global });
			}
			else if (instruction.is<Use *>())
			{
				auto use = instruction.as<Use *>();

				uses.insert(use->fileName());
			}
		}
	}
}

antlrcpp::Any Visitor::visitProgram(fluencParser::ProgramContext *context)
{
	std::vector<CallableNode *> roots;
	std::multimap<std::string, CallableNode *> functions;
	std::map<std::string, const BaseValue *> locals;
	std::map<std::string, const Node *> globals;
	std::map<std::string, Prototype *> types;
	std::unordered_set<std::string> uses;

	auto instructions = context->instruction();

	std::vector<antlrcpp::Any> results;

	std::transform(begin(instructions), end(instructions), std::back_inserter(results), [this](auto instruction)
	{
		return fluencBaseVisitor::visit(instruction);
	});

	populateInstructions(std::vector<std::string>()
		, results
		, roots
		, functions
		, locals
		, globals
		, types
		, uses
		);

	return ModuleInfo
	{
		roots,
		functions,
		locals,
		globals,
		types,
		uses,
	};
}

FunctionAttribute getAttribute(fluencParser::FunctionContext *ctx)
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

antlrcpp::Any Visitor::visitFunction(fluencParser::FunctionContext *context)
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

		Visitor visitor(m_namespaces, nullptr, terminator, nullptr);

		auto entryPoint = new ExportedFunctionNode(name
			, visitor.visit<Node *>(block)
			, visitor.visit<ITypeName *>(context->typeName())
			);

		return static_cast<CallableNode *>(entryPoint);
	}

	auto iteratorType = new IteratorType();

	auto terminator = new TerminatorNode(name, attribute);

	Visitor visitor(m_namespaces, iteratorType, terminator, nullptr);

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

antlrcpp::Any Visitor::visitRegularType(fluencParser::RegularTypeContext *context)
{
	auto qualifiedName = qualifiedNames(m_namespaces
		, context->getText()
		);

	return static_cast<ITypeName *>(
		new DzTypeName(context
			, qualifiedName
		)
	);
}

antlrcpp::Any Visitor::visitFunctionType(fluencParser::FunctionTypeContext *context)
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

antlrcpp::Any Visitor::visitStandardArgument(fluencParser::StandardArgumentContext *context)
{
	auto argument = new DzArgument(context->ID()->getText()
		, visit<ITypeName *>(context->typeName())
		);

	return static_cast<DzBaseArgument *>(argument);
}

antlrcpp::Any Visitor::visitTupleArgument(fluencParser::TupleArgumentContext *context)
{
	auto arguments = context->argument();

	std::vector<DzBaseArgument *> tupleArguments;

	std::transform(begin(arguments), end(arguments), std::back_insert_iterator(tupleArguments), [this](fluencParser::ArgumentContext *argument)
	{
		return visit<DzBaseArgument *>(argument);
	});

	auto argument = new DzTupleArgument(tupleArguments);

	return static_cast<DzBaseArgument *>(argument);
}

antlrcpp::Any Visitor::visitRet(fluencParser::RetContext *context)
{
	if (context->chained)
	{
		auto continuation = visit<Node *>(context->chained);

		auto ret = new ReturnNode(m_iteratorType, m_alpha, continuation);

		Visitor visitor(m_namespaces, nullptr, ret, nullptr);

		auto value = visitor
			.visit<Node *>(context->value);

		auto instruction = new BlockInstructionNode(value, true);

		return static_cast<Node *>(instruction);
	}

	auto ret = new ReturnNode(m_iteratorType, m_alpha, nullptr);

	Visitor visitor(m_namespaces, nullptr, ret, nullptr);

	auto value = visitor
		.visit<Node *>(context->value);

	auto instruction = new BlockInstructionNode(value, false);

	return static_cast<Node *>(instruction);
}

antlrcpp::Any Visitor::visitBlock(fluencParser::BlockContext *context)
{
	auto expressions = context->expression();

	auto ret = visit<Node *, BlockInstructionNode *>(context->ret());

	auto result = std::accumulate(rbegin(expressions), rend(expressions), ret, [this](BlockInstructionNode *consumer, fluencParser::ExpressionContext *expression)
	{
		auto stackFrame = new BlockStackFrameNode(consumer);

		Visitor visitor(m_namespaces, m_iteratorType, stackFrame, m_alpha);

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

antlrcpp::Any Visitor::visitBinary(fluencParser::BinaryContext *context)
{
	auto binary = new BinaryNode(m_alpha
		, context->OP()->getText()
		);

	Visitor leftVisitor(m_namespaces, m_iteratorType, binary, nullptr);

	auto left = leftVisitor
		.visit<Node *>(context->left);

	Visitor rightVisitor(m_namespaces, m_iteratorType, left, nullptr);

	auto right = rightVisitor
		.visit<Node *>(context->right);

	return right;
}

antlrcpp::Any Visitor::visitCall(fluencParser::CallContext *context)
{
	auto expression = context->expression();

	auto names = qualifiedNames(m_namespaces
		, context->ID()->getText()
		);

	auto call = new FunctionCallNode(context, names);

	std::vector<Node *> values;

	std::transform(begin(expression), end(expression), std::back_insert_iterator(values), [this](fluencParser::ExpressionContext *parameter)
	{
		auto sink = new ReferenceSinkNode(TerminatorNode::instance());

		Visitor visitor(m_namespaces, m_iteratorType, sink, nullptr);

		return visitor
			.visit<Node *>(parameter);
	});

	auto evaluation = new LazyEvaluationNode(call);
	auto withEvaluation = new StackSegmentNode(values, evaluation, TerminatorNode::instance());
	auto withoutEvaluation = new StackSegmentNode(values, call, TerminatorNode::instance());
	auto proxy = new FunctionCallProxyNode(names, m_alpha, withEvaluation, withoutEvaluation);

	return static_cast<Node *>(proxy);
}

antlrcpp::Any Visitor::visitWith(fluencParser::WithContext *context)
{
	auto assignments = context->assignment();

	std::vector<std::string> fields;

	std::transform(begin(assignments), end(assignments), std::back_insert_iterator(fields), [](fluencParser::AssignmentContext *assignment)
	{
		return assignment->field()->ID()->getText();
	});

	auto instantiation = new InstantiationNode(context
		, fields
		, WithPrototypeProvider::instance()
		, m_alpha
		);

	return std::accumulate(begin(assignments), end(assignments), static_cast<Node *>(instantiation), [this](auto consumer, fluencParser::AssignmentContext *assignment)
	{
		Visitor visitor(m_namespaces, m_iteratorType, consumer, nullptr);

		return visitor
			.visit<Node *>(assignment->expression());
	});
}

antlrcpp::Any Visitor::visitMember(fluencParser::MemberContext *context)
{
	auto access = context->ID();

	auto path = std::accumulate(begin(access) + 1, end(access), (*begin(access))->getText(), [](auto name, antlr4::tree::TerminalNode *node)
	{
		std::stringstream ss;
		ss << name << "." << node->getText();

		return ss.str();
	});

	auto qualifiedPath = qualifiedNames(m_namespaces, path);

	auto with = context->with();

	if (with)
	{
		auto member = new MemberAccessNode(context, visit<Node *>(with), qualifiedPath);

		return static_cast<Node *>(member);
	}

	auto member = new MemberAccessNode(context, m_alpha, qualifiedPath);

	return static_cast<Node *>(member);
}

antlrcpp::Any Visitor::visitInt32Literal(fluencParser::Int32LiteralContext *context)
{
	auto constant = new IntegralLiteralNode(m_alpha
		, DzTypeName::int32()
		, context->INT()->getText()
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any Visitor::visitInt64Literal(fluencParser::Int64LiteralContext *context)
{
	auto constant = new IntegralLiteralNode(m_alpha
		, DzTypeName::int64()
		, context->INT()->getText()
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any Visitor::visitBoolLiteral(fluencParser::BoolLiteralContext *context)
{
	auto constant = new BooleanLiteralNode(m_alpha
		, context->BOOL()->getText()
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any Visitor::visitStringLiteral(fluencParser::StringLiteralContext *context)
{
	auto constant = new StringLiteralNode(m_alpha
		, context->STRING()->getText()
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any Visitor::visitUint32Literal(fluencParser::Uint32LiteralContext *context)
{
	auto constant = new IntegralLiteralNode(m_alpha
		, DzTypeName::uint32()
		, context->INT()->getText()
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any Visitor::visitStructure(fluencParser::StructureContext *context)
{
	auto name = context->name->getText();
	auto inputFields = context->field();

	std::vector<PrototypeFieldEmbryo> fields;

	std::transform(begin(inputFields), end(inputFields), std::back_insert_iterator(fields), [this](fluencParser::FieldContext *field) -> PrototypeFieldEmbryo
	{
		auto name = field->ID()->getText();

		ITypeName *type = nullptr;

		if (field->typeName())
		{
			type = visit<ITypeName *>(field->typeName());
		}

		if (field->expression())
		{
			Visitor visitor(m_namespaces, m_iteratorType, TerminatorNode::instance(), nullptr);

			auto defaultValue = visitor
				.visit<Node *>(field->expression());

			return { name, defaultValue, type };
		}

		return { name, nullptr, type };
	});

	std::vector<ITypeName *> parentTypes;

	auto parentTypeNames = context->typeName();

	std::transform(begin(parentTypeNames), end(parentTypeNames), std::back_insert_iterator(parentTypes), [this](fluencParser::TypeNameContext *typeName) -> ITypeName *
	{
		return visit<ITypeName *>(typeName);
	});

	return new Prototype(name, fields, parentTypes);
}

antlrcpp::Any Visitor::visitInstantiation(fluencParser::InstantiationContext *context)
{
	auto assignments = context->assignment();

	std::vector<std::string> fields;

	std::transform(begin(assignments), end(assignments), std::back_insert_iterator(fields), [](fluencParser::AssignmentContext *assignment)
	{
		return assignment->field()->ID()->getText();
	});

	auto typeName = visit<ITypeName *>(context->typeName());

	auto prototypeProvider = new DefaultPrototypeProvider(typeName);
	auto instantiation = new InstantiationNode(context
		, fields
		, prototypeProvider
		, m_alpha
		);

	return std::accumulate(begin(assignments), end(assignments), static_cast<Node *>(instantiation), [this](auto consumer, fluencParser::AssignmentContext *assignment)
	{
		Visitor visitor(m_namespaces, m_iteratorType, consumer, nullptr);

		return visitor
			.visit<Node *>(assignment->expression());
	});
}

antlrcpp::Any Visitor::visitConditional(fluencParser::ConditionalContext *context)
{
	Visitor blockVisitor(m_namespaces, m_iteratorType, m_beta, nullptr);

	auto block = blockVisitor
		.visit<Node *, BlockInstructionNode *>(context->block());

	auto conditional = new ConditionalNode(m_alpha, block);

	Visitor expressionVisitor(m_namespaces, m_iteratorType, conditional, nullptr);

	auto condition = expressionVisitor
		.visit<Node *>(context->expression());

	auto instruction = new BlockInstructionNode(condition
		, block->containsIterator()
		);

	return static_cast<Node *>(instruction);
}

antlrcpp::Any Visitor::visitGlobal(fluencParser::GlobalContext *context)
{
	auto name = context->ID()->getText();

	Visitor visitor(m_namespaces, m_iteratorType, TerminatorNode::instance(), nullptr);

	auto literal = visitor
		.visit<Node *>(context->expression());

	return new GlobalNode(literal, name);
}

antlrcpp::Any Visitor::visitNothing(fluencParser::NothingContext *context)
{
	UNUSED(context);

	auto constant = new IntegralLiteralNode(m_alpha
		, DzTypeName::without()
		, "0"
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any Visitor::visitGroup(fluencParser::GroupContext *context)
{
	return visit<Node *>(context->expression());
}

antlrcpp::Any Visitor::visitExpansion(fluencParser::ExpansionContext *context)
{
	auto expansion = new ExpansionNode(context, m_alpha);

	Visitor visitor(m_namespaces, m_iteratorType, expansion, nullptr);

	return visitor
		.visit<Node *>(context->expression());
}

antlrcpp::Any Visitor::visitContinuation(fluencParser::ContinuationContext *context)
{
	auto continuation = new ContinuationNode();

	auto expressions = context->expression();

	return std::accumulate(begin(expressions), end(expressions), static_cast<Node *>(continuation), [this](Node *consumer, fluencParser::ExpressionContext *parameter)
	{
		Visitor visitor(m_namespaces, m_iteratorType, consumer, nullptr);

		auto result = visitor
			.visit<Node *>(parameter);

		return result;
	});
}

antlrcpp::Any Visitor::visitArray(fluencParser::ArrayContext *context)
{
	auto expressions = context->expression();

	if (expressions.empty())
	{
		auto empty = new EmptyArrayNode(TerminatorNode::instance());

		return static_cast<Node *>(empty);
	}

	std::vector<Indexed<fluencParser::ExpressionContext *>> indexed;

	std::transform(begin(expressions), end(expressions), index_iterator(0u), std::back_insert_iterator(indexed), [=](auto x, auto y) -> Indexed<fluencParser::ExpressionContext *>
	{
		return { y, x };
	});

	auto firstValue = std::accumulate(begin(indexed), end(indexed), (Node *)TerminatorNode::instance(), [&](auto next, Indexed<fluencParser::ExpressionContext *> expression)
	{
		auto indexSink = new IndexSinkNode(expression.index, next);
		auto referenceSink = new ReferenceSinkNode(indexSink);

		Visitor visitor(m_namespaces, m_iteratorType, referenceSink, nullptr);

		return visitor
			.visit<Node *>(expression.value);
	});

	auto lazySink = new ArraySinkNode(expressions.size(), m_alpha, firstValue);

	return static_cast<Node *>(lazySink);
}

antlrcpp::Any Visitor::visitCharLiteral(fluencParser::CharLiteralContext *context)
{
	auto value = new CharacterLiteralNode(m_alpha
		, context->CHARACTER()->getText()
		);

	return static_cast<Node *>(value);
}

antlrcpp::Any Visitor::visitByteLiteral(fluencParser::ByteLiteralContext *context)
{
	auto constant = new IntegralLiteralNode(m_alpha
		, DzTypeName::byte()
		, context->INT()->getText()
		);

	return static_cast<Node *>(constant);
}

antlrcpp::Any Visitor::visitLocal(fluencParser::LocalContext *context)
{
	auto local = new LocalNode(m_alpha
		, context->ID()->getText()
		);

	Visitor visitor(m_namespaces, m_iteratorType, local, nullptr);

	return visitor.visit<Node *>(context->expression());
}

antlrcpp::Any Visitor::visitInstruction(fluencParser::InstructionContext *context)
{
	return visitChildren(context);
}

antlrcpp::Any Visitor::visitNs(fluencParser::NsContext *context)
{
	auto instructions = context->instruction();
	auto name = context->ID()->getText();

	auto namespaces = m_namespaces;

	namespaces.push_back(name);

	Visitor visitor(namespaces
		, m_iteratorType
		, m_alpha
		, m_beta
		);

	std::vector<antlrcpp::Any> children;

	std::transform(begin(instructions), end(instructions), std::back_inserter(children), [&](auto instruction)
	{
		return visitor.visitAny(instruction);
	});

	return new Namespace(children, name);
}

antlrcpp::Any Visitor::visitUse(fluencParser::UseContext *context)
{
	return new Use(context->STRING()->getText());
}
