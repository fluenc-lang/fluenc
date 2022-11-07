#ifndef RENDEREDNODE_H
#define RENDEREDNODE_H

#include <vector>

#include "Node.h"

class RenderedNode : public Node
{
	public:
		RenderedNode(const std::vector<DzResult> &results);

		std::vector<DzResult> accept(const DefaultNodeVisitor &emitter, DefaultVisitorContext context) const override;

	private:
		std::vector<DzResult> m_results;
};


#endif // RENDEREDNODE_H
