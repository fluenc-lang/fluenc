#include "values/ReadOnlyIteratorValue.h"

ReadOnlyIteratorValue::ReadOnlyIteratorValue(const std::vector<DzResult> &results)
	: m_results(results)
{
}

std::vector<DzResult> ReadOnlyIteratorValue::accept(const Emitter &emitter, DefaultVisitorContext context) const
{
	UNUSED(emitter);
	UNUSED(context);

	return m_results;
}
