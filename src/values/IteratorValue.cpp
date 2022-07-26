#include "values/IteratorValue.h"

IteratorValue::IteratorValue(const EntryPoint *entryPoint
	, const Node *subject
	)
	: m_entryPoint(entryPoint)
	, m_subject(subject)
{
}

std::vector<DzResult> IteratorValue::accept(const Emitter &emitter, const EntryPoint &entryPoint, Stack values) const
{
	return emitter.visitIteratorValue(this, entryPoint, values);
}
