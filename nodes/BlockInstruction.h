#ifndef BLOCKINSTRUCTION_H
#define BLOCKINSTRUCTION_H

#include "Node.h"

class BlockInstruction : public Node
{
	public:
		BlockInstruction(const Node *subject, bool containsIterator);

		bool containsIterator() const;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Node *m_subject;

		bool m_containsIterator;
};

#endif // BLOCKINSTRUCTION_H
