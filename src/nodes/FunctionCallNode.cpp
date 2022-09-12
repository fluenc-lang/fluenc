#include "nodes/FunctionCallNode.h"
#include "nodes/CallableNode.h"

FunctionCallNode::FunctionCallNode(const std::shared_ptr<peg::Ast> &ast, const std::vector<std::string> &names, const Node *evaluation)
	: m_ast(ast)
	, m_names(names)
	, m_evaluation(evaluation)
{
}

int FunctionCallNode::order(const EntryPoint &entryPoint) const
{
	auto &functions = entryPoint.functions();

	for (auto &name : m_names)
	{
		auto iterator = functions.find(name);

		if (iterator != functions.end())
		{
			if (iterator->second->attribute() == FunctionAttribute::Iterator)
			{
				return 1;
			}
		}
	}

	return -1;
}

std::vector<DzResult> FunctionCallNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visit(this, context);
}

std::vector<DzResult> FunctionCallNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visit(this, context);
}

const Node *FunctionCallNode::accept(const NodeLocator &visitor, DummyVisitorContext context) const
{
	return visitor.visit(this, context);
}
