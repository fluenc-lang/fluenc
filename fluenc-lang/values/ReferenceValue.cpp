#include "values/ReferenceValue.h"

#include "EntryPoint.h"
#include "IRBuilderEx.h"

ReferenceValue::ReferenceValue(const Type *type, llvm::Value *value)
	: m_type(type)
	, m_value(value)
{
}

ValueId ReferenceValue::id() const
{
	return ValueId::Reference;
}

const Type *ReferenceValue::type() const
{
	return m_type;
}

const BaseValue *ReferenceValue::clone(const EntryPoint &entryPoint, CloneStrategy strategy) const
{
	auto alloc = entryPoint.alloc(m_type);

	if (strategy == CloneStrategy::Value)
	{
		IRBuilderEx builder(entryPoint);

		auto load = builder.createLoad(this);

		builder.createStore(load, alloc);
	}

	return alloc;
}

ReferenceValue::operator llvm::Value *() const
{
	return m_value;
}
