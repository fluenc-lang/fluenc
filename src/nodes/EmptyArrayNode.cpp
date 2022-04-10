#include "nodes/EmptyArrayNode.h"

#include "values/WithoutValue.h"
#include "values/TaintedValue.h"

EmptyArrayNode::EmptyArrayNode(Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> EmptyArrayNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto tainted = new TaintedValue(WithoutValue::instance());

	values.push(tainted);

	return m_consumer->build(entryPoint, values);
}
