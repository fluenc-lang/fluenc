#include <llvm/IR/Constant.h>

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

llvm::Constant *TypedValue::constant() const
{
	return static_cast<llvm::Constant *>(m_value);
}

TypedValue::operator llvm::Value *() const
{
	return m_value;
}
