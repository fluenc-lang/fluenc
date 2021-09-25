#include "TypedValue.h"

TypedValue::TypedValue()
	: m_type(nullptr)
	, m_value(nullptr)
{
}

TypedValue::TypedValue(Type *type, llvm::Value *value)
	: m_type(type)
	, m_value(value)
{
}

Type *TypedValue::type() const
{
	return m_type;
}

TypedValue::operator llvm::Value *() const
{
	return m_value;
}
