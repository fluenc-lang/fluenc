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

std::vector<DzResult> LazyValue::build(llvm::BasicBlock *block, const Stack &values) const
{
	UNUSED(values);

	auto entryPoint = m_entryPoint->withBlock(block);

	return m_subject->build(entryPoint, m_values);
}

const Type *LazyValue::type() const
{
	return m_iteratorType;
}

const BaseValue *LazyValue::clone(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return this;
}
