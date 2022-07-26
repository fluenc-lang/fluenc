#ifndef BLOCKSTACKFRAMENODE_H
#define BLOCKSTACKFRAMENODE_H

#include "Node.h"

class BlockStackFrameNode : public Node
{
	friend class Emitter;

	public:
		BlockStackFrameNode(Node *consumer);

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;

	private:
		Node *m_consumer;
};

#endif // BLOCKSTACKFRAMENODE_H
