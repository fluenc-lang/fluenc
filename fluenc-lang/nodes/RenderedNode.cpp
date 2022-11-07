#include "Utility.h"

#include "nodes/RenderedNode.h"

RenderedNode::RenderedNode(const std::vector<DzResult> &results)
	: m_results(results)
{
}

std::vector<DzResult> RenderedNode::accept(const DefaultNodeVisitor &emitter, DefaultVisitorContext context) const
{
	UNUSED(emitter);
	UNUSED(context);

	return m_results;
}
