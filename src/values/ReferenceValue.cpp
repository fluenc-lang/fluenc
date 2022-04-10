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
	return entryPoint.alloc(m_type);
}

ReferenceValue::operator llvm::Value *() const
{
	return m_value;
}
