#include "values/IteratorValue.h"

IteratorValue::IteratorValue(const EntryPoint *entryPoint
	, const Node *subject
	)
	: m_entryPoint(entryPoint)
	, m_subject(subject)
{
}

std::vector<DzResult > IteratorValue::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitIteratorValue(this, context);
}

std::vector<DzResult > IteratorValue::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitIteratorValue(this, context);
}
