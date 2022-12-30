#include "ArrayValueProxy.h"
#include "ReferenceValue.h"

ArrayValueProxy::ArrayValueProxy(const ReferenceValue *index, const IIteratable *subject)
	: m_index(index)
	, m_subject(subject)
{
}

std::vector<DzResult> ArrayValueProxy::accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const
{
	context.values.push(m_index);

	return m_subject->accept(visitor, context);
}
