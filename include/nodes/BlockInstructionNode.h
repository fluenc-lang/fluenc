#ifndef BLOCKINSTRUCTIONNODE_H
#define BLOCKINSTRUCTIONNODE_H

#include "Node.h"

class BlockInstructionNode : public Node
{
	public:
		BlockInstructionNode(const Node *subject, bool containsIterator);

		bool containsIterator() const;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Node *m_subject;

		bool m_containsIterator;
};

#endif // BLOCKINSTRUCTIONNODE_H
