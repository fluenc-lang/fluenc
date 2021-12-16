#include "LazyValue.h"
#include "EntryPoint.h"
#include "DzTypeName.h"

#include "types/IteratorType.h"

LazyValue::LazyValue(const DzValue *subject, const EntryPoint &entryPoint)
	: m_subject(subject)
	, m_entryPoint(new EntryPoint(entryPoint))
{
}

std::vector<DzResult> LazyValue::build(llvm::BasicBlock *block, const Stack &values) const
{
	auto entryPoint = m_entryPoint->withBlock(block);

	return m_subject->build(entryPoint, values);
}

const Type *LazyValue::type() const
{
	return IteratorType::instance();
}
