#include "BlockInstruction.h"

BlockInstruction::BlockInstruction(const Node *subject, bool containsIterator)
	: m_subject(subject)
	, m_containsIterator(containsIterator)
{
}

bool BlockInstruction::containsIterator() const
{
	return m_containsIterator;
}

std::vector<DzResult> BlockInstruction::build(const EntryPoint &entryPoint, Stack values) const
{
	return m_subject->build(entryPoint, values);
}
