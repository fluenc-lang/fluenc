#include "nodes/UnaryNode.h"

#include "values/ForwardedValue.h"

UnaryNode::UnaryNode(const Node *consumer, const std::string &op)
	: m_consumer(consumer)
	, m_op(op)
{
}

std::vector<DzResult> UnaryNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto operand = values.pop();
	auto value = resolveOp(operand);

	values.push(value);

	return m_consumer->build(entryPoint, values);
}

const BaseValue *UnaryNode::resolveOp(const BaseValue *value) const
{
	if (m_op == "@")
	{
		return new ForwardedValue(value);
	}

	throw new std::exception(); // TODO
}
