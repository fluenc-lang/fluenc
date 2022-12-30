#include "values/IteratorValue.h"

IteratorValue::IteratorValue(const EntryPoint *entryPoint
	, const Node *subject
	)
	: m_entryPoint(entryPoint)
	, m_subject(subject)
{
}

std::vector<DzResult > IteratorValue::accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const
{
	return visitor.visit(this, context);
}
