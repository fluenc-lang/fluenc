#include <algorithm>
#include <numeric>

#include "nodes/FunctionNode.h"

#include "DzArgument.h"

FunctionNode::FunctionNode(FunctionAttribute attribute
	, const std::string &name
	, const std::vector<DzBaseArgument *> &arguments
	, Node *block
	)
	: m_attribute(attribute)
	, m_name(name)
	, m_arguments(arguments)
	, m_block(block)
{
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
	return m_attribute;
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

std::vector<DzResult<BaseValue>> FunctionNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitFunction(this, entryPoint, values);
}

std::vector<DzResult<BaseValue> > FunctionNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitFunction(this, entryPoint, values);
}
