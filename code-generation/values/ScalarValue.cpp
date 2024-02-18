#include <llvm/IR/Constant.h>

#include "ScalarValue.h"
#include "ReferenceValue.h"
#include "EntryPoint.h"

scalar_value::scalar_value(const Type *type, llvm::Value *value)
	: m_type(type)
	, m_value(value)
{
}

value_id scalar_value::id() const
{
    return value_id::scalar;
}

const Type *scalar_value::type() const
{
	return m_type;
}

const BaseValue *scalar_value::clone(const EntryPoint &entryPoint, clone_strategy strategy) const
{
    if (strategy == clone_strategy::Storage)
	{
		return entryPoint.alloc(m_type);
	}

	return this;
}

llvm::Constant *scalar_value::constant() const
{
	return static_cast<llvm::Constant *>(m_value);
}

scalar_value::operator llvm::Value *() const
{
	return m_value;
}
