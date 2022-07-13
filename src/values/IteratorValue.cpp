#include "values/IteratorValue.h"

IteratorValue::IteratorValue(const EntryPoint *entryPoint
	, const Node *subject
	)
	: m_entryPoint(entryPoint)
	, m_subject(subject)
{
}

std::vector<DzResult<BaseValue> > IteratorValue::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitIteratorValue(this, entryPoint, values);
}

std::vector<DzResult<BaseValue> > IteratorValue::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitIteratorValue(this, entryPoint, values);
}
