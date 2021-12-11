#include "WithoutValue.h"

#include "types/WithoutType.h"

const WithoutValue *WithoutValue::instance()
{
	static WithoutValue instance;

	return &instance;
}

const Type *WithoutValue::type() const
{
	return WithoutType::instance();
}
