#include "WithoutValue.h"
#include "utility.hpp"

#include "types/WithoutType.h"

const without_value *without_value::instance()
{
    static without_value instance;

	return &instance;
}

value_id without_value::id() const
{
    return value_id::without;
}

const Type *without_value::type() const
{
    return without_type::instance();
}

const BaseValue *without_value::clone(const EntryPoint &entryPoint, clone_strategy strategy) const
{
	UNUSED(entryPoint);
	UNUSED(strategy);

	return this;
}
