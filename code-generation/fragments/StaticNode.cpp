#include "StaticNode.h"
#include "Emitter.h"

StaticNode::StaticNode(const BaseValue *value, const fluenc::expression_t &consumer, const fragment *next)
	: m_value(value)
	, m_consumer(consumer)
	, m_next(next)
{
}

std::vector<DzResult> StaticNode::build(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visit(this, context);
}
