#include <llvm/IR/Constant.h>

#include "ScalarValue.h"
#include "ReferenceValue.h"
#include "Utility.h"
#include "EntryPoint.h"

ScalarValue::ScalarValue(const Type *type, llvm::Value *value)
	: m_type(type)
	, m_value(value)
{
}

ValueId ScalarValue::id() const
{
	return ValueId::Scalar;
}

const Type *ScalarValue::type() const
{
	return m_type;
}

const BaseValue *ScalarValue::clone(const EntryPoint &entryPoint, CloneStrategy strategy) const
{
	if (strategy == CloneStrategy::Storage)
	{
		return entryPoint.alloc(m_type);
	}

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
