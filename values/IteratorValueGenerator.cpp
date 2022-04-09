#include "IteratorValueGenerator.h"
#include "IteratorValue.h"

#include "types/IteratorType.h"

IteratorValueGenerator::IteratorValueGenerator(const Type *iteratorType, const Node *subject, const EntryPoint &entryPoint)
	: m_iteratorType(iteratorType)
	, m_subject(subject)
	, m_entryPoint(new EntryPoint(entryPoint))
{
}

const IIteratable *IteratorValueGenerator::generate(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return new IteratorValue(m_entryPoint, m_subject);
}

const ILazyValueGenerator *IteratorValueGenerator::clone(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return this;
}

const Type *IteratorValueGenerator::type() const
{
	return m_iteratorType;
}
