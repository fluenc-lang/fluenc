#ifndef BLOCKSTACKFRAME_H
#define BLOCKSTACKFRAME_H

#include "Node.h"

class BlockStackFrame : public Node
{
	public:
		BlockStackFrame(Node *consumer);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		Node *m_consumer;
};

#endif // BLOCKSTACKFRAME_H
