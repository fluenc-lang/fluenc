#include "Utility.h"

#include "values/IteratorValueGeneratorProxy.h"
#include "values/ReadOnlyIteratorValue.h"
#include "values/RenderedIteratorValue.h"

IteratorValueGeneratorProxy::IteratorValueGeneratorProxy(const ILazyValueGenerator *subject, EntryPoint *entryPoint, const std::vector<DzResult> &results)
	: m_strategies({ &IteratorValueGeneratorProxy::useRendered, &IteratorValueGeneratorProxy::createNew })
	, m_subject(subject)
	, m_entryPoint(entryPoint)
	, m_results(results)
{
}

const IIteratable *IteratorValueGeneratorProxy::generate(const EntryPoint &entryPoint, GenerationMode mode) const
{
	if (mode == GenerationMode::DryRun)
	{
		return new ReadOnlyIteratorValue(m_results);
	}

	auto strategy = m_strategies.get();

	return (this->*strategy)(entryPoint, mode);
}

const ILazyValueGenerator *IteratorValueGeneratorProxy::clone(const EntryPoint &entryPoint, CloneStrategy strategy) const
{
	UNUSED(entryPoint);
	UNUSED(strategy);

	return this;
}

const Type *IteratorValueGeneratorProxy::type() const
{
	return m_subject->type();
}

const IIteratable *IteratorValueGeneratorProxy::createNew(const EntryPoint &entryPoint, GenerationMode mode) const
{
	return m_subject->generate(entryPoint, mode);
}

const IIteratable *IteratorValueGeneratorProxy::useRendered(const EntryPoint &entryPoint, GenerationMode mode) const
{
	UNUSED(entryPoint);
	UNUSED(mode);

	return new RenderedIteratorValue(m_entryPoint, m_results);
}
