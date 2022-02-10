#include "DzBlockInstruction.h"

DzBlockInstruction::DzBlockInstruction(const DzValue *subject, bool containsIterator)
	: m_subject(subject)
	, m_containsIterator(containsIterator)
{
}

bool DzBlockInstruction::containsIterator() const
{
	return m_containsIterator;
}

std::vector<DzResult> DzBlockInstruction::build(const EntryPoint &entryPoint, Stack values) const
{
	return m_subject->build(entryPoint, values);
}
