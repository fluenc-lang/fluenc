#include "LazyValue.h"
#include "EntryPoint.h"
#include "DzTypeName.h"

#include "types/IteratorType.h"

LazyValue::LazyValue(const Stack &values
	, const EntryPoint &entryPoint
	, const Type *iteratorType
	, const DzValue *subject
	)
	: m_values(values)
	, m_entryPoint(new EntryPoint(entryPoint))
	, m_iteratorType(iteratorType)
	, m_subject(subject)
{
}

std::vector<DzResult> LazyValue::build(const EntryPoint &entryPoint, const Stack &values) const
{
	UNUSED(values);

	auto locals = entryPoint.locals();

	for (auto &[key, value] : m_entryPoint->locals())
	{
		locals[key] = value;
	}

	auto ep = (*m_entryPoint)
		.withBlock(entryPoint.block())
		.withLocals(locals);

	return m_subject->build(ep, m_values);
}

const Type *LazyValue::type() const
{
	return IteratorType::instance();
//	return m_iteratorType;
}

const BaseValue *LazyValue::clone(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return this;
}
