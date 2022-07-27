#ifndef BLOCKSTACKFRAMENODE_H
#define BLOCKSTACKFRAMENODE_H

#include "Node.h"

class BlockStackFrameNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		BlockStackFrameNode(Node *consumer);

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const override;

	private:
		Node *m_consumer;
};

#endif // BLOCKSTACKFRAMENODE_H
