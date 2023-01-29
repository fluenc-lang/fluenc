#ifndef TUPLESINKNODE_H
#define TUPLESINKNODE_H

#include "Node.h"

class TupleSinkNode : public Node
{
	public:
		TupleSinkNode(size_t size, const Node *consumer);

		std::vector<DzResult> accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const override;

	private:
		size_t m_size;

		const Node *m_consumer;
};

#endif // TUPLESINKNODE_H
