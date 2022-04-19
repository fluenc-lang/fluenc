#include "nodes/NothingNode.h"

#include "values/WithoutValue.h"

NothingNode::NothingNode(const Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> NothingNode::build(const EntryPoint &entryPoint, Stack values) const
{
	values.push(WithoutValue::instance());

	return m_consumer->build(entryPoint, values);
}
