#include "nodes/ConditionalNode.h"

ConditionalNode::ConditionalNode(const Node *ifFalse, const Node *ifTrue)
	: m_ifTrue(ifTrue)
	, m_ifFalse(ifFalse)
{
}
