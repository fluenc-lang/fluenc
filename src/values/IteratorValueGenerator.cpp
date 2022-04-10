#include "values/IteratorValueGenerator.h"
#include "values/IteratorValue.h"

#include "types/IteratorType.h"

IteratorValueGenerator::IteratorValueGenerator(const Node *subject, const EntryPoint &entryPoint)
	: m_subject(subject)
	, m_entryPoint(new EntryPoint(entryPoint))
{
}

const IIteratable *IteratorValueGenerator::generate(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return new IteratorValue(m_entryPoint, m_subject);
}

const Type *IteratorValueGenerator::type() const
{
	return IteratorType::instance();
}
