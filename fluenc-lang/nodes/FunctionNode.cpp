#include <algorithm>
#include <numeric>

#include "nodes/FunctionNode.h"
#include "nodes/TerminatorNode.h"
#include "nodes/IBlockInstruction.h"

#include "Visitor.h"
#include "DzBaseArgument.h"
#include "Utility.h"
#include "Type.h"

FunctionNode::FunctionNode(const std::string &name
	, const std::vector<DzBaseArgument *> &arguments
	, const std::vector<std::shared_ptr<peg::AstBase<peg::EmptyType> > > &nodes
	, const std::vector<std::string> &namespaces
	, const Type *iteratorType
	)
	: m_name(name)
	, m_arguments(arguments)
	, m_block(inject(nodes, namespaces, iteratorType, this))
{
}

IBlockInstruction *FunctionNode::inject(const std::vector<std::shared_ptr<peg::AstBase<peg::EmptyType>>> &nodes
	, const std::vector<std::string> &namespaces
	, const Type *iteratorType
	, const Node *self
	)
{
	Visitor visitor(namespaces, iteratorType, self, TerminatorNode::instance(), nullptr);

	return visitor.visitBlock(*nodes.rbegin());
}

std::string FunctionNode::name() const
{
	return m_name;
}

std::vector<DzBaseArgument *> FunctionNode::arguments() const
{
	return m_arguments;
}

FunctionAttribute FunctionNode::attribute() const
{
	if (m_block->containsIterator())
	{
		return FunctionAttribute::Iterator;
	}

	return FunctionAttribute::None;
}

int8_t FunctionNode::signatureCompatibility(const EntryPoint &entryPoint, const std::vector<const Type *> &values) const
{
	if (m_arguments.size() != values.size())
	{
		return -1;
	}

	std::vector<std::pair<const Type *, const Type *>> types;

	std::transform(begin(m_arguments), end(m_arguments), begin(values), std::back_inserter(types), [=](DzBaseArgument *argument, auto valueType)
	{
		auto argumentType = argument->type(entryPoint);

		return std::make_pair(argumentType, valueType);
	});

	return std::accumulate(begin(types), end(types), 0, [=](auto score, auto pair) -> int8_t
	{
		if (score < 0)
		{
			return score;
		}

		auto [argumentType, valueType] = pair;

		auto compatibility = valueType->compatibility(argumentType, entryPoint);

		if (compatibility < 0)
		{
			return compatibility;
		}

		return score + compatibility;
	});
}

std::vector<DzResult> FunctionNode::accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const
{
	return visitor.visit(this, context);
}
