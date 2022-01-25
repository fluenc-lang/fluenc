#include "ReferenceValue.h"
#include "EntryPoint.h"

ReferenceValue::ReferenceValue(const Type *type, llvm::Value *value)
	: m_type(type)
	, m_value(value)
{
}

const Type *ReferenceValue::type() const
{
	return m_type;
}

const BaseValue *ReferenceValue::clone(const EntryPoint &entryPoint) const
{
	auto &context = entryPoint.context();

	auto storageType = m_type->storageType(*context);
	auto alloc = entryPoint.alloc(storageType);

	return new ReferenceValue(m_type, alloc);
}

ReferenceValue::operator llvm::Value *() const
{
	return m_value;
}
