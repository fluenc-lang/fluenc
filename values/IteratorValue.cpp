#include "IteratorValue.h"
#include "EntryPoint.h"
#include "DzTypeName.h"

#include "types/IteratorType.h"

IteratorValue::IteratorValue(const EntryPoint *entryPoint
	, const Node *subject
	)
	: m_entryPoint(entryPoint)
	, m_subject(subject)
{
}

std::vector<DzResult> IteratorValue::build(const EntryPoint &entryPoint) const
{
	auto locals = entryPoint.locals();

	for (auto &[key, value] : m_entryPoint->locals())
	{
		locals[key] = value;
	}

	auto ep = (*m_entryPoint)
		.withBlock(entryPoint.block())
		.withLocals(locals)
		.withIteratorStorage(entryPoint.iteratorStorage());

	return m_subject->build(ep, Stack());
}
