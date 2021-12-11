#include <llvm/IR/Instructions.h>

#include "NamedValue.h"
#include "EntryPoint.h"
#include "DzTypeName.h"

#include "values/TypedValue.h"

NamedValue::NamedValue(const std::string &name
	, const BaseValue *value
	, const Type *type
	)
	: m_name(name)
	, m_value(value)
	, m_type(type)
{
}

std::string NamedValue::name() const
{
	return m_name;
}

const BaseValue *NamedValue::value() const
{
	return m_value;
}

const Type *NamedValue::type() const
{
	return m_type;
}

bool NamedValue::hasValue() const
{
	return m_value;
}
