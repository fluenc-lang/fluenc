#include "EntryPoint.h"
#include "utility.hpp"

#include "values/IteratorValueGenerator.h"
#include "fragments/IteratorValue.h"

#include "types/IteratorType.h"

IteratorValueGenerator::IteratorValueGenerator(const Type *iteratorType, stack_segment_node *subject, const EntryPoint &entryPoint)
	: m_iteratorType(iteratorType)
	, m_subject(subject)
	, m_entryPoint(new EntryPoint(entryPoint))
{
}

const fragment *IteratorValueGenerator::generate(const EntryPoint &entryPoint, GenerationMode mode) const
{
	UNUSED(entryPoint);
	UNUSED(mode);

	return new IteratorValue(m_entryPoint, m_subject);
}

const ILazyValueGenerator *IteratorValueGenerator::clone(const EntryPoint &entryPoint, clone_strategy strategy) const
{
	UNUSED(entryPoint);
	UNUSED(strategy);

	return this;
}

const Type *IteratorValueGenerator::type() const
{
	return m_iteratorType;
}

stack_segment_node *IteratorValueGenerator::subject() const
{
	return m_subject;
}
