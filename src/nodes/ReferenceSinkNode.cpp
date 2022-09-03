#include "nodes/ReferenceSinkNode.h"

ReferenceSinkNode::ReferenceSinkNode(const Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> ReferenceSinkNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitReferenceSink(this, context);
}

std::vector<DzResult> ReferenceSinkNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitReferenceSink(this, context);
}
