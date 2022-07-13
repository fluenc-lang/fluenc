#ifndef BLOCKSTACKFRAMENODE_H
#define BLOCKSTACKFRAMENODE_H

#include "Node.h"

class BlockStackFrameNode : public Node
{
	friend class Emitter;

	public:
		BlockStackFrameNode(Node *consumer);

		std::vector<DzResult<BaseValue>> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;

	private:
		Node *m_consumer;
};

#endif // BLOCKSTACKFRAMENODE_H
