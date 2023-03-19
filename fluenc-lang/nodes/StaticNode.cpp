#include "StaticNode.h"

StaticNode::StaticNode(const BaseValue *value, const Node *consumer)
	: m_value(value)
	, m_consumer(consumer)
{
}

std::vector<DzResult> StaticNode::accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const
{
	context.values.push(m_value);

	return m_consumer->accept(visitor, context);
}
