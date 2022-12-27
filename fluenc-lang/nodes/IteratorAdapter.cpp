#include "IteratorAdapter.h"

#include "values/IIteratable.h"

IteratorAdapter::IteratorAdapter(const IIteratable *subject)
	: m_subject(subject)
{
}

std::vector<DzResult> IteratorAdapter::accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const
{
	return m_subject->accept(visitor, context);
}
