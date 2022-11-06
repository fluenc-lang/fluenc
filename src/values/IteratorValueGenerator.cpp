#include "values/IteratorValueGenerator.h"
#include "values/IteratorValue.h"

#include "types/IteratorType.h"

IteratorValueGenerator::IteratorValueGenerator(const Type *iteratorType, const Node *subject, const EntryPoint &entryPoint)
	: m_iteratorType(iteratorType)
	, m_subject(subject)
	, m_entryPoint(new EntryPoint(entryPoint))
{
}

const IIteratable *IteratorValueGenerator::generate(const EntryPoint &entryPoint, GenerationMode mode) const
{
	UNUSED(entryPoint);
	UNUSED(mode);

	return new IteratorValue(m_entryPoint, m_subject);
}

const ILazyValueGenerator *IteratorValueGenerator::clone(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return this;
}

const ILazyValueGenerator *IteratorValueGenerator::forward(size_t id) const
{
	UNUSED(id);

	return this;
}

const Type *IteratorValueGenerator::type() const
{
	return m_iteratorType;
}

const Node *IteratorValueGenerator::subject() const
{
	return m_subject;
}
