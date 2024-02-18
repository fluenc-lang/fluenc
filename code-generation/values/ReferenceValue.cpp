#include "values/ReferenceValue.h"

#include "EntryPoint.h"
#include "IRBuilderEx.h"

reference_value::reference_value(const Type *type, llvm::Value *value)
	: m_type(type)
	, m_value(value)
{
}

value_id reference_value::id() const
{
    return value_id::reference;
}

const Type *reference_value::type() const
{
	return m_type;
}

const BaseValue *reference_value::clone(const EntryPoint &entryPoint, clone_strategy strategy) const
{
	auto alloc = entryPoint.alloc(m_type);

        if (strategy == clone_strategy::Value)
	{
		IRBuilderEx builder(entryPoint);

		auto load = builder.createLoad(this);

		builder.createStore(load, alloc);
	}

	return alloc;
}

reference_value::operator llvm::Value *() const
{
	return m_value;
}
