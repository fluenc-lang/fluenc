#include "IteratorValue.h"
#include "Emitter.h"

IteratorValue::IteratorValue(const EntryPoint *entryPoint
	 , stack_segment_node *subject
	)
	: m_entryPoint(entryPoint)
	, m_subject(subject)
{
}

std::vector<DzResult > IteratorValue::build(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visit(this, context);
}
