#include "ArrayValueProxy.h"
#include "Emitter.h"

#include "values/ReferenceValue.h"

ArrayValueProxy::ArrayValueProxy(const reference_value *index, const fragment *subject)
	: m_index(index)
	, m_subject(subject)
{
}

std::vector<DzResult> ArrayValueProxy::build(const Emitter &visitor, DefaultVisitorContext context) const
{
	context.values.push(m_index);

	return m_subject->build(visitor, context);
}
