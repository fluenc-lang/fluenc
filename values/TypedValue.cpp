#include <llvm/IR/Constant.h>

#include "TypedValue.h"
#include "Utility.h"

TypedValue::TypedValue(const Type *type, llvm::Value *value)
	: m_type(type)
	, m_value(value)
{
}

const Type *TypedValue::type() const
{
	return m_type;
}

const BaseValue *TypedValue::clone(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return this;
}

llvm::Constant *TypedValue::constant() const
{
	return static_cast<llvm::Constant *>(m_value);
}

TypedValue::operator llvm::Value *() const
{
	return m_value;
}
