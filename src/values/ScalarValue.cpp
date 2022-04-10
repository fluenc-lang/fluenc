#include <llvm/IR/Constant.h>

#include "values/ScalarValue.h"
#include "Utility.h"

ScalarValue::ScalarValue(const Type *type, llvm::Value *value)
	: m_type(type)
	, m_value(value)
{
}

const Type *ScalarValue::type() const
{
	return m_type;
}

const BaseValue *ScalarValue::clone(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return this;
}

llvm::Constant *ScalarValue::constant() const
{
	return static_cast<llvm::Constant *>(m_value);
}

ScalarValue::operator llvm::Value *() const
{
	return m_value;
}
