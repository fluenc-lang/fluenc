#include "TypedValue.h"

TypedValue::TypedValue()
	: m_value(nullptr)
	, m_type(nullptr)
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
