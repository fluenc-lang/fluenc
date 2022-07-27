#include "values/IteratorValue.h"

IteratorValue::IteratorValue(const EntryPoint *entryPoint
	, const Node *subject
	)
	: m_entryPoint(entryPoint)
	, m_subject(subject)
{
}

std::vector<DzResult > IteratorValue::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitIteratorValue(this, entryPoint, values);
}

std::vector<DzResult > IteratorValue::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitIteratorValue(this, entryPoint, values);
}
