#include "IteratorValueGenerator.h"
#include "IteratorValue.h"

IteratorValueGenerator::IteratorValueGenerator(const DzValue *subject, const EntryPoint &entryPoint)
	: m_subject(subject)
	, m_entryPoint(new EntryPoint(entryPoint))
{
}

IIteratable *IteratorValueGenerator::generate(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return new IteratorValue(m_entryPoint, m_subject);
}
