#include "nodes/CapsuleNode.h"

CapsuleNode::CapsuleNode(const Stack &values, const Node *subject)
	: m_values(values)
	, m_call(subject)
{
}
