#include "AllocatorNode.h"

DefaultNode::DefaultNode(const Type *type, const fluenc::expression_t &consumer)
	: m_type(type)
	, m_consumer(consumer)
{
}
