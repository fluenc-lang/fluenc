#include "EmptyArrayNode.h"

#include "values/WithoutValue.h"

EmptyArrayNode::EmptyArrayNode(Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> EmptyArrayNode::build(const EntryPoint &entryPoint, Stack values) const
{
	values.push(WithoutValue::instance());

	return m_consumer->build(entryPoint, values);
}
