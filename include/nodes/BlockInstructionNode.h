#ifndef BLOCKINSTRUCTIONNODE_H
#define BLOCKINSTRUCTIONNODE_H

#include "IBlockInstruction.h"

class BlockInstructionNode : public IBlockInstruction
{
	friend class Emitter;
	friend class Analyzer;

	public:
		BlockInstructionNode(const Node *subject, bool containsIterator);

		bool containsIterator() const override;

		std::vector<DzResult<BaseValue>> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;

	private:
		const Node *m_subject;

		bool m_containsIterator;
};

#endif // BLOCKINSTRUCTIONNODE_H
