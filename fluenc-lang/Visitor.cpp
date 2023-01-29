#include <vector>
#include <map>
#include <unordered_set>
#include <numeric>

#include "peglib.h"

#include "Visitor.h"
#include "ModuleInfo.h"
#include "Namespace.h"
#include "Use.h"
#include "DzTypeName.h"
#include "FunctionTypeName.h"
#include "DzArgument.h"
#include "DzTupleArgument.h"
#include "DefaultPrototypeProvider.h"
#include "Indexed.h"
#include "IndexIterator.h"
#include "WithPrototypeProvider.h"
#include "CallableNode.h"

#include "nodes/GlobalNode.h"
#include "nodes/TerminatorNode.h"
#include "nodes/BlockInstructionNode.h"
#include "nodes/FunctionNode.h"
#include "nodes/ImportedFunctionNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/ContinuationNode.h"
#include "nodes/ExportedFunctionTerminatorNode.h"
#include "nodes/ExportedFunctionNode.h"
#include "nodes/IntegralLiteralNode.h"
#include "nodes/BooleanLiteralNode.h"
#include "nodes/NothingNode.h"
#include "nodes/StringLiteralNode.h"
#include "nodes/CharacterLiteralNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/InstantiationNode.h"
#include "nodes/ConditionalNode.h"
#include "nodes/ArraySinkNode.h"
#include "nodes/EmptyArrayNode.h"
#include "nodes/FunctionCallNode.h"
#include "nodes/MemberAccessNode.h"
#include "nodes/ReferenceSinkNode.h"
#include "nodes/TailFunctionCallNode.h"
#include "nodes/StackSegmentNode.h"
#include "nodes/PreEvaluationNode.h"
#include "nodes/PostEvaluationNode.h"
#include "nodes/IndexSinkNode.h"
#include "nodes/ExpansionNode.h"
#include "nodes/LocalNode.h"
#include "nodes/UnaryNode.h"
#include "nodes/FunctionCallProxyNode.h"
#include "nodes/FloatLiteralNode.h"
#include "nodes/JunctionNode.h"
#include "nodes/DistributorNode.h"
#include "nodes/BlockStackFrameNode.h"
#include "nodes/TupleSinkNode.h"

#include "types/Prototype.h"
#include "types/IteratorType.h"

class CallableNode;
class BaseValue;
class Prototype;

Visitor::Visitor(const std::vector<std::string> &namespaces
	, const Type *iteratorType
	, const Node *parent
	, const Node *alpha
	, const Node *beta
	)
	: m_namespaces(namespaces)
	, m_iteratorType(iteratorType)
	, m_parent(parent)
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

	if (qualifiedName.str() == name)
	{
		return { name };
	}

	return { qualifiedName.str(), name };
}

void populateInstructions(const std::vector<std::string> &namespaces
	, const std::vector<std::any> &instructions
	, std::vector<const CallableNode *> &roots
	, std::multimap<std::string, const CallableNode *> &functions
	, std::map<std::string, const BaseValue *> &locals
	, std::map<std::string, const Node *> &globals
	, std::map<std::string, Prototype *> &types
	, std::unordered_map<std::string, Use *> &uses
	)
{
	for (auto &instruction : instructions)
	{
		if (instruction.type() == typeid(Namespace *))
		{
			auto _namespace = std::any_cast<Namespace *>(instruction);

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
			if (instruction.type() == typeid(CallableNode *))
			{
				auto callable = std::any_cast<CallableNode *>(instruction);

				if (callable->attribute() == FunctionAttribute::Export)
				{
					roots.push_back(callable);

					auto import = new ImportedFunctionNode(callable->returnType(), callable->name(), nullptr, callable->arguments());

					auto name = qualifiedNames(namespaces
						, callable->name()
						);

					functions.insert({ name[0], import });
				}
				else
				{
					auto name = qualifiedNames(namespaces
						, callable->name()
						);

					functions.insert({ name[0], callable });
				}
			}
			else if (instruction.type() == typeid(Prototype *))
			{
				auto prototype = std::any_cast<Prototype *>(instruction);

				auto name = qualifiedNames(namespaces
					, prototype->name()
					);

				types.insert({ name[0], prototype });
			}
			else if (instruction.type() == typeid(GlobalNode *))
			{
				auto global = std::any_cast<GlobalNode *>(instruction);

				auto name = qualifiedNames(namespaces
					, global->name()
					);

				globals.insert({ name[0], global });
			}
			else if (instruction.type() == typeid(Use *))
			{
				auto use = std::any_cast<Use *>(instruction);

				uses.insert({ use->fileName(), use });
			}
		}
	}
}

ModuleInfo Visitor::visit(const std::shared_ptr<peg::Ast> &ast) const
{
	std::vector<const CallableNode *> roots;
	std::multimap<std::string, const CallableNode *> functions;
	std::map<std::string, const BaseValue *> locals;
	std::map<std::string, const Node *> globals;
	std::map<std::string, Prototype *> types;
	std::unordered_map<std::string, Use *> uses;

	std::vector<std::any> results;

	std::transform(begin(ast->nodes), end(ast->nodes), std::back_inserter(results), [this](auto instruction)
	{
		return visitInstruction(instruction);
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

std::any Visitor::visitInstruction(const std::shared_ptr<peg::Ast> &ast) const
{
	using namespace peg::udl;

	switch (ast->tag)
	{
		case "Instruction"_:
			return visitInstruction(ast->nodes[0]);
		case "Function"_:
			return visitFunction(ast);
		case "Structure"_:
			return visitStructure(ast);
		case "Global"_:
			return visitGlobal(ast);
		case "Namespace"_:
			return visitNamespace(ast);
		case "Use"_:
			return visitUse(ast);
		case "Block"_:
			return visitBlock(ast);
	}

	throw new std::exception();
}

std::string Visitor::visitId(const std::shared_ptr<peg::Ast> &ast) const
{
	return ast->token_to_string();
}

std::string Visitor::visitInteger(const std::shared_ptr<peg::Ast> &ast) const
{
	return ast->token_to_string();
}

std::string Visitor::visitString(const std::shared_ptr<peg::Ast> &ast) const
{
	static std::unordered_map<char, char> table =
	{
		{ 'n', '\n' },
		{ 'r', '\r' },
		{ 't', '\t' },
		{ '0', '\0' },
		{ '\\', '\\' },
	};

	auto input = ast->token_to_string();

	std::ostringstream stream;

	std::accumulate(begin(input), end(input), false, [&](auto substitute, auto token)
	{
		if (substitute)
		{
			if (auto iterator = table.find(token); iterator != end(table))
			{
				stream << iterator->second;
			}
		}
		else
		{
			if (token == '\\')
			{
				return true;
			}
			else
			{
				stream << token;
			}
		}

		return false;
	});

	return stream.str();
}

std::vector<ITypeName *> Visitor::visitTypeList(const std::shared_ptr<peg::Ast> &ast) const
{
	std::vector<ITypeName *> results;

	std::transform(begin(ast->nodes), end(ast->nodes), std::back_inserter(results), [this](auto node)
	{
		return visitTypeName(node);
	});

	return results;
}

std::vector<PrototypeFieldEmbryo> Visitor::visitFieldList(const std::shared_ptr<peg::Ast> &ast) const
{
	std::vector<PrototypeFieldEmbryo> results;

	std::transform(begin(ast->nodes), end(ast->nodes), std::back_inserter(results), [this](auto node)
	{
		return visitField(node);
	});

	return results;
}

std::vector<std::string> Visitor::visitIdList(const std::shared_ptr<peg::Ast> &ast) const
{
	std::vector<std::string> results;

	std::transform(begin(ast->nodes), end(ast->nodes), std::back_inserter(results), [this](auto node)
	{
		return visitId(node);
	});

	return results;
}

Node *Visitor::visitExpression(const std::shared_ptr<peg::Ast> &ast) const
{
	using namespace peg::udl;

	switch (ast->tag)
	{
		case "Expression"_:
			return visitExpression(ast->nodes[0]);
		case "Literal"_:
			return visitLiteral(ast);
		case "Binary"_:
			return visitBinary(ast);
		case "Unary"_:
			return visitUnary(ast);
		case "Member"_:
			return visitMember(ast);
		case "Call"_:
			return visitCall(ast);
		case "Instantiation"_:
			return visitInstantiation(ast);
		case "Conditional"_:
			return visitConditional(ast);
		case "Array"_:
			return visitArray(ast);
		case "Tuple"_:
			return visitTuple(ast);
		case "Group"_:
			return visitGroup(ast);
		case "Expansion"_:
			return visitExpansion(ast);
		case "Local"_:
			return visitLocal(ast);
		case "Tail"_:
			return visitTail(ast);
	}

	throw new std::exception();
}

Node *Visitor::visitLiteral(const std::shared_ptr<peg::Ast> &ast) const
{
	using namespace peg::udl;

	switch (ast->tag)
	{
		case "Literal"_:
			return visitLiteral(ast->nodes[0]);
		case "Int32Literal"_:
			return visitInt32Literal(ast);
		case "Int64Literal"_:
			return visitInt64Literal(ast);
		case "Float32Literal"_:
			return visitFloat32Literal(ast);
		case "BooleanLiteral"_:
			return visitBooleanLiteral(ast);
		case "StringLiteral"_:
			return visitStringLiteral(ast);
		case "Uint32Literal"_:
			return visitUint32Literal(ast);
		case "CharLiteral"_:
			return visitCharLiteral(ast);
		case "ByteLiteral"_:
			return visitByteLiteral(ast);
		case "NothingLiteral"_:
			return visitNothingLiteral(ast);
	}

	throw new std::exception();
}

Node *Visitor::visitInt32Literal(const std::shared_ptr<peg::Ast> &ast) const
{
	return new IntegralLiteralNode(m_alpha
		, DzTypeName::int32()
		, visitInteger(ast->nodes[0])
		);
}

Node *Visitor::visitInt64Literal(const std::shared_ptr<peg::Ast> &ast) const
{
	return new IntegralLiteralNode(m_alpha
		, DzTypeName::int64()
		, visitInteger(ast->nodes[0])
		);
}

Node *Visitor::visitFloat32Literal(const std::shared_ptr<peg::Ast> &ast) const
{
	return new FloatLiteralNode(m_alpha
		, DzTypeName::f32()
		, ast->token_to_string()
		);
}

Node *Visitor::visitBooleanLiteral(const std::shared_ptr<peg::Ast> &ast) const
{
	return new BooleanLiteralNode(m_alpha
		, ast->token_to_string()
		);
}

Node *Visitor::visitStringLiteral(const std::shared_ptr<peg::Ast> &ast) const
{
	return new StringLiteralNode(m_alpha
		, visitString(ast->nodes[0])
		);
}

Node *Visitor::visitUint32Literal(const std::shared_ptr<peg::Ast> &ast) const
{
	return new IntegralLiteralNode(m_alpha
		, DzTypeName::uint32()
		, visitInteger(ast->nodes[0])
		);
}

Node *Visitor::visitCharLiteral(const std::shared_ptr<peg::Ast> &ast) const
{
	return new CharacterLiteralNode(m_alpha
		, visitString(ast)
		);
}

Node *Visitor::visitByteLiteral(const std::shared_ptr<peg::Ast> &ast) const
{
	return new IntegralLiteralNode(m_alpha
		, DzTypeName::byte()
		, visitInteger(ast->nodes[0])
		);
}

Node *Visitor::visitNothingLiteral(const std::shared_ptr<peg::Ast> &ast) const
{
	UNUSED(ast);

	return new NothingNode(m_alpha);
}

Node *Visitor::visitBinary(const std::shared_ptr<peg::Ast> &ast) const
{
	auto binary = new BinaryNode();
	binary->ast = ast;
	binary->op = visitId(ast->nodes[1]);
	binary->consumer = m_alpha;

	Visitor leftVisitor(m_namespaces, m_iteratorType, m_parent, binary, nullptr);

	auto left = leftVisitor
		.visitExpression(ast->nodes[0]);

	Visitor rightVisitor(m_namespaces, m_iteratorType, m_parent, left, nullptr);

	auto right = rightVisitor
		.visitExpression(ast->nodes[2]);

	return right;
}

Node *Visitor::visitMember(const std::shared_ptr<peg::Ast> &ast) const
{
	auto access = visitIdList(ast->nodes[0]);

	auto path = std::accumulate(begin(access) + 1, end(access), (*begin(access)), [](auto name, std::string node)
	{
		std::stringstream ss;
		ss << name << "." << node;

		return ss.str();
	});

	auto qualifiedPath = qualifiedNames(m_namespaces, path);

	if (ast->nodes.size() > 1)
	{
		auto with = visitWith(ast->nodes[1]);

		return new MemberAccessNode(with, ast, qualifiedPath);
	}

	return new MemberAccessNode(m_alpha, ast, qualifiedPath);
}

Node *Visitor::visitCall(const std::shared_ptr<peg::Ast> &ast) const
{
	auto sink = new ReferenceSinkNode(TerminatorNode::instance());
	auto preEvaluation = new PreEvaluationNode(sink);

	Visitor visitor(m_namespaces, m_iteratorType, m_parent, preEvaluation, nullptr);

	std::vector<Node *> values;

	std::transform(begin(ast->nodes) + 1, end(ast->nodes), std::back_inserter(values), [&](auto node)
	{
		return visitor.visitExpression(node);
	});

	auto names = qualifiedNames(m_namespaces
		, visitId(ast->nodes[0])
		);

	auto postEvaluation = new PostEvaluationNode();
	auto call = new FunctionCallNode(ast, names, postEvaluation);

	auto segment = new StackSegmentNode(values, call, TerminatorNode::instance());

	return new FunctionCallProxyNode(names, segment, m_alpha);
}

Node *Visitor::visitInstantiation(const std::shared_ptr<peg::Ast> &ast) const
{
	auto typeName = visitTypeName(ast->nodes[0]);

	std::vector<std::string> fields;

	std::transform(begin(ast->nodes) + 1, end(ast->nodes), std::back_insert_iterator(fields), [this](auto assignment)
	{
		return visitId(assignment->nodes[0]);
	});

	auto prototypeProvider = new DefaultPrototypeProvider(typeName);
	auto instantiation = new InstantiationNode(m_alpha
		, prototypeProvider
		, ast
		, fields
		);

	return std::accumulate(begin(ast->nodes) + 1, end(ast->nodes), static_cast<Node *>(instantiation), [this](auto consumer, auto assignment)
	{
		Visitor visitor(m_namespaces, m_iteratorType, m_parent, consumer, nullptr);

		return visitor
			.visitExpression(assignment->nodes[1]);
	});
}

Node *Visitor::visitConditional(const std::shared_ptr<peg::Ast> &ast) const
{
	Visitor blockVisitor(m_namespaces, m_iteratorType, m_parent, m_beta, nullptr);

	auto block = blockVisitor
		.visitBlock(ast->nodes[1]);

	auto conditional = new ConditionalNode(m_alpha, block);

	Visitor expressionVisitor(m_namespaces, m_iteratorType, m_parent, conditional, nullptr);

	auto condition = expressionVisitor
		.visitExpression(ast->nodes[0]);

	return new BlockInstructionNode(condition
		, block->containsIterator()
		);
}

Node *Visitor::visitArray(const std::shared_ptr<peg::Ast> &ast) const
{
	if (ast->nodes.empty())
	{
		auto empty = new EmptyArrayNode(TerminatorNode::instance());

		return static_cast<Node *>(empty);
	}

	std::vector<Indexed<std::shared_ptr<peg::Ast>>> indexed;

	std::transform(begin(ast->nodes), end(ast->nodes), index_iterator(0u), std::back_insert_iterator(indexed), [=](auto x, auto y) -> Indexed<std::shared_ptr<peg::Ast>>
	{
		return { y, x };
	});

	auto firstValue = std::accumulate(begin(indexed), end(indexed), static_cast<Node *>(TerminatorNode::instance()), [&](auto next, Indexed<std::shared_ptr<peg::Ast>> expression)
	{
		auto indexSink = new IndexSinkNode(expression.index, next);
		auto referenceSink = new ReferenceSinkNode(indexSink);

		Visitor visitor(m_namespaces, m_iteratorType, m_parent, referenceSink, nullptr);

		return visitor
			.visitExpression(expression.value);
	});

	return new ArraySinkNode(ast->nodes.size(), ast, m_alpha, firstValue);
}

Node *Visitor::visitTuple(const std::shared_ptr<peg::Ast> &ast) const
{
	auto sink = new TupleSinkNode(ast->nodes.size(), m_alpha);

	return std::accumulate(begin(ast->nodes), end(ast->nodes), static_cast<Node *>(sink), [&](auto consumer, auto value)
	{
		Visitor visitor(m_namespaces, m_iteratorType, m_parent, consumer, nullptr);

		return visitor.visitExpression(value);
	});
}

Node *Visitor::visitGroup(const std::shared_ptr<peg::Ast> &ast) const
{
	return visitExpression(ast->nodes[0]);
}

Node *Visitor::visitExpansion(const std::shared_ptr<peg::Ast> &ast) const
{
	auto expansion = new ExpansionNode(m_alpha, ast);

	Visitor visitor(m_namespaces, m_iteratorType, m_parent, expansion, nullptr);

	return visitor
		.visitExpression(ast->nodes[0]);
}

Node *Visitor::visitLocal(const std::shared_ptr<peg::Ast> &ast) const
{
	auto local = new LocalNode(m_alpha
		, visitId(ast->nodes[0])
		);

	Visitor visitor(m_namespaces, m_iteratorType, m_parent, local, nullptr);

	return visitor.visitExpression(ast->nodes[1]);
}

Node *Visitor::visitContinuation(const std::shared_ptr<peg::Ast> &ast) const
{
	auto continuation = new ContinuationNode(m_parent, m_iteratorType);
	auto sink = new ReferenceSinkNode(continuation);

	return std::accumulate(begin(ast->nodes) + 1, end(ast->nodes), static_cast<Node *>(sink), [this](Node *consumer, auto parameter)
	{
		Visitor visitor(m_namespaces, m_iteratorType, m_parent, consumer, nullptr);

		auto result = visitor
			.visitExpression(parameter);

		return result;
	});
}

Node *Visitor::visitWith(const std::shared_ptr<peg::Ast> &ast) const
{
	std::vector<std::string> fields;

	std::transform(begin(ast->nodes), end(ast->nodes), std::back_insert_iterator(fields), [this](auto assignment)
	{
		return visitId(assignment->nodes[0]);
	});

	auto instantiation = new InstantiationNode(m_alpha
		, WithPrototypeProvider::instance()
		, ast
		, fields
		);

	return std::accumulate(begin(ast->nodes), end(ast->nodes), static_cast<Node *>(instantiation), [this](auto consumer, auto assignment)
	{
		Visitor visitor(m_namespaces, m_iteratorType, m_parent, consumer, nullptr);

		return visitor
			.visitExpression(assignment->nodes[1]);
	});
}

Node *Visitor::visitUnary(const std::shared_ptr<peg::Ast> &ast) const
{
	UNUSED(ast);

	auto unary = new UnaryNode();
	unary->ast = ast;
	unary->consumer = m_alpha;
	unary->op = visitId(ast->nodes[0]);

	Visitor visitor(m_namespaces, m_iteratorType, m_parent, unary, nullptr);

	return visitor
			.visitExpression(ast->nodes[1]);
}

Node *Visitor::visitTail(const std::shared_ptr<peg::Ast> &ast) const
{
	auto sink = new ReferenceSinkNode(TerminatorNode::instance());
	auto preEvaluation = new PreEvaluationNode(sink);

	Visitor visitor(m_namespaces, m_iteratorType, m_parent, preEvaluation, nullptr);

	std::vector<Node *> values;

	std::transform(begin(ast->nodes) + 1, end(ast->nodes), std::back_inserter(values), [&](auto node)
	{
		return visitor.visitExpression(node);
	});

	auto names = qualifiedNames(m_namespaces
		, visitId(ast->nodes[0])
		);

	auto postEvaluation = new PostEvaluationNode();
	auto call = new FunctionCallNode(ast, names, postEvaluation);
	auto proxy = new TailFunctionCallNode(names, call);

	return new StackSegmentNode(values, proxy, m_alpha);
}

CallableNode *Visitor::visitFunction(const std::shared_ptr<peg::Ast> &ast) const
{
	using namespace peg::udl;

	switch (ast->tag)
	{
		case "Function"_:
			return visitFunction(ast->nodes[0]);
		case "RegularFunction"_:
			return visitRegularFunction(ast);
		case "ImportedFunction"_:
			return visitImportedFunction(ast);
		case "ExportedFunction"_:
			return visitExportedFunction(ast);
	}

	throw new std::exception();
}

CallableNode *Visitor::visitRegularFunction(const std::shared_ptr<peg::Ast> &ast) const
{
	auto name = visitId(ast->nodes[0]);

	auto qualifiedName = qualifiedNames(m_namespaces, name);

	std::vector<DzBaseArgument *> arguments;

	std::transform(begin(ast->nodes) + 1, end(ast->nodes) - 1, std::back_inserter(arguments), [this](auto argument)
	{
		return visitArgument(argument);
	});

	auto iteratorType = new IteratorType();

	auto evaluation = new PostEvaluationNode();
	auto call = new FunctionCallNode(ast->nodes[0], qualifiedName, evaluation);

	Visitor visitor(m_namespaces, iteratorType, call, TerminatorNode::instance(), nullptr);

	auto block = visitor.visitBlock(*ast->nodes.rbegin());

	return new FunctionNode(name, arguments, block);
}

CallableNode *Visitor::visitExportedFunction(const std::shared_ptr<peg::Ast> &ast) const
{
	auto returnType = visitTypeName(ast->nodes[0]);
	auto name = visitId(ast->nodes[1]);

	std::vector<DzBaseArgument *> arguments;

	std::transform(begin(ast->nodes) + 2, end(ast->nodes) - 1, std::back_inserter(arguments), [this](auto argument)
	{
		return visitArgument(argument);
	});

	auto terminator = new ExportedFunctionTerminatorNode();

	Visitor visitor(m_namespaces, nullptr, m_parent, TerminatorNode::instance(), nullptr);

	auto block = visitor.visitBlock(*ast->nodes.rbegin());

	auto junction = new JunctionNode(block);
	auto distributor = new DistributorNode(junction, terminator);

	return new ExportedFunctionNode(name, arguments, distributor, returnType);
}

CallableNode *Visitor::visitImportedFunction(const std::shared_ptr<peg::Ast> &ast) const
{
	auto returnType = visitTypeName(ast->nodes[0]);
	auto name = visitId(ast->nodes[1]);

	std::vector<DzBaseArgument *> arguments;

	std::transform(begin(ast->nodes) + 2, end(ast->nodes), std::back_inserter(arguments), [this](auto argument)
	{
		return visitArgument(argument);
	});

	return new ImportedFunctionNode(returnType, name, ast, arguments);
}

DzBaseArgument *Visitor::visitArgument(const std::shared_ptr<peg::Ast> &ast) const
{
	using namespace peg::udl;

	switch (ast->tag)
	{
		case "Argument"_:
			return visitArgument(ast->nodes[0]);
		case "StandardArgument"_:
			return visitStandardArgument(ast);
		case "TupleArgument"_:
			return visitTupleArgument(ast);
	}

	throw new std::exception();
}

DzBaseArgument *Visitor::visitStandardArgument(const std::shared_ptr<peg::Ast> &ast) const
{
	auto type = visitTypeName(ast->nodes[0]);
	auto name = visitId(ast->nodes[1]);

	return new DzArgument(name, type);
}

DzBaseArgument *Visitor::visitTupleArgument(const std::shared_ptr<peg::Ast> &ast) const
{
	std::vector<DzBaseArgument *> arguments;

	std::transform(begin(ast->nodes), end(ast->nodes), std::back_inserter(arguments), [this](auto node)
	{
		return visitArgument(node);
	});

	return new DzTupleArgument(arguments);
}

Prototype *Visitor::visitStructure(const std::shared_ptr<peg::Ast> &ast) const
{
	auto name = visitId(ast->nodes[0]);
	auto parentTypes = visitTypeList(ast->nodes[1]);
	auto fields = visitFieldList(ast->nodes[2]);

	return new Prototype(name, fields, parentTypes);
}

GlobalNode *Visitor::visitGlobal(const std::shared_ptr<peg::Ast> &ast) const
{
	Visitor visitor(m_namespaces, m_iteratorType, m_parent, TerminatorNode::instance(), nullptr);

	auto literal = visitor
		.visitExpression(ast->nodes[1]);

	return new GlobalNode(literal
		, visitId(ast->nodes[0])
		);
}

Namespace *Visitor::visitNamespace(const std::shared_ptr<peg::Ast> &ast) const
{
	auto name = ast->nodes[0]->token_to_string();
	auto namespaces = m_namespaces;

	namespaces.push_back(name);

	Visitor visitor(namespaces
		, m_iteratorType
		, m_parent
		, m_alpha
		, m_beta
		);

	std::vector<std::any> children;

	std::transform(begin(ast->nodes) + 1, end(ast->nodes), std::back_inserter(children), [&](auto instruction)
	{
		return visitor.visitInstruction(instruction);
	});

	return new Namespace(children, name);
}

Use *Visitor::visitUse(const std::shared_ptr<peg::Ast> &ast) const
{
	return new Use(ast, visitString(ast->nodes[0]));
}

IBlockInstruction *Visitor::visitReturn(const std::shared_ptr<peg::Ast> &ast) const
{
	if (ast->nodes.size() > 1)
	{
		auto continuation = visitContinuation(ast->nodes[1]);

		auto ret = new ReturnNode(m_iteratorType, m_alpha, continuation);

		Visitor visitor(m_namespaces, nullptr, m_parent, ret, nullptr);

		auto value = visitor
			.visitExpression(ast->nodes[0]);

		return new BlockInstructionNode(value, true);
	}

	auto ret = new ReturnNode(m_iteratorType, m_alpha, nullptr);

	Visitor visitor(m_namespaces, nullptr, m_parent, ret, nullptr);

	auto value = visitor
		.visitExpression(ast->nodes[0]);

	return new BlockInstructionNode(value, false);
}

IBlockInstruction *Visitor::visitBlock(const std::shared_ptr<peg::Ast> &ast) const
{
	Visitor visitor(m_namespaces, m_iteratorType, m_parent, m_alpha, m_beta);

	auto first = rbegin(ast->nodes);

	auto ret = visitor.visitReturn(*first);

	return std::accumulate(first + 1, rend(ast->nodes), ret, [&](IBlockInstruction *consumer, auto expression) -> IBlockInstruction *
	{
		auto stackFrame = new BlockStackFrameNode(consumer);

		Visitor visitor(m_namespaces, m_iteratorType, m_parent, stackFrame, m_alpha);

		auto value = visitor
			.visitExpression(expression);

		if (auto instruction = dynamic_cast<const IBlockInstruction *>(value))
		{
			return new BlockInstructionNode(instruction
				, instruction->containsIterator() || consumer->containsIterator()
				);
		}

		return new BlockInstructionNode(value
			, consumer->containsIterator()
			);
	});
}

ITypeName *Visitor::visitTypeName(const std::shared_ptr<peg::Ast> &ast) const
{
	using namespace peg::udl;

	switch (ast->tag)
	{
		case "TypeName"_:
			return visitTypeName(ast->nodes[0]);
		case "RegularType"_:
			return visitRegularType(ast);
		case "FunctionType"_:
			return visitFunctionType(ast);
	}

	return nullptr;
}

ITypeName *Visitor::visitRegularType(const std::shared_ptr<peg::Ast> &ast) const
{
	auto qualifiedName = qualifiedNames(m_namespaces
		, visitId(ast->nodes[0])
		);

	return new DzTypeName(ast, qualifiedName);
}

ITypeName *Visitor::visitFunctionType(const std::shared_ptr<peg::Ast> &ast) const
{
	std::vector<const ITypeName *> types;

	std::transform(begin(ast->nodes), end(ast->nodes), std::back_inserter(types), [this](auto typeName)
	{
		return visitRegularType(typeName);
	});

	return new FunctionTypeName(types);
}

PrototypeFieldEmbryo Visitor::visitField(const std::shared_ptr<peg::Ast> &ast) const
{
	using namespace peg::udl;

	switch (ast->tag)
	{
		case "Field"_:
			return visitField(ast->nodes[0]);
		case "StandardField"_:
			return visitStandardField(ast);
		case "DecoratedField"_:
			return visitDecoratedField(ast);
	}

	throw new std::exception();
}

PrototypeFieldEmbryo Visitor::visitStandardField(const std::shared_ptr<peg::Ast> &ast) const
{
	auto name = visitId(ast->nodes[0]);

	if (ast->nodes.size() > 1)
	{
		Visitor visitor(m_namespaces, m_iteratorType, m_parent, TerminatorNode::instance(), nullptr);

		auto defaultValue = visitor
			.visitExpression(ast->nodes[1]);

		return { name, defaultValue, nullptr };
	}

	return { name, nullptr, nullptr };
}

PrototypeFieldEmbryo Visitor::visitDecoratedField(const std::shared_ptr<peg::Ast> &ast) const
{
	auto type = visitTypeName(ast->nodes[0]);
	auto name = visitId(ast->nodes[1]);

	if (ast->nodes.size() > 2)
	{
		Visitor visitor(m_namespaces, m_iteratorType, m_parent, TerminatorNode::instance(), nullptr);

		auto defaultValue = visitor
			.visitExpression(ast->nodes[2]);

		return { name, defaultValue, type };
	}

	return { name, nullptr, type };
}
