#ifndef BLOCKSTACKFRAMENODE_H
#define BLOCKSTACKFRAMENODE_H

#include "Node.h"

class BlockStackFrameNode : public VisitableNode<BlockStackFrameNode>
{
	friend class Emitter;

	public:
		BlockStackFrameNode(Node *consumer);

	private:
		Node *m_consumer;
};

#endif // BLOCKSTACKFRAMENODE_H
