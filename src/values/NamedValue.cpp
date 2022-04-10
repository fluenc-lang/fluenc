#include <llvm/IR/Instructions.h>

#include "values/NamedValue.h"
#include "EntryPoint.h"
#include "DzTypeName.h"

#include "values/ScalarValue.h"

NamedValue::NamedValue(const std::string &name, const BaseValue *value)
	: m_name(name)
	, m_value(value)
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
	return m_value->type();
}

const BaseValue *NamedValue::clone(const EntryPoint &entryPoint) const
{
	return new NamedValue(m_name
		, m_value->clone(entryPoint)
		);
}
