#include "values/FunctionValue.h"
#include "EntryPoint.h"
#include "DzBaseArgument.h"

#include "nodes/CallableNode.h"

#include "types/FunctionType.h"

FunctionValue::FunctionValue(const CallableNode *function, const EntryPoint &entryPoint)
	: m_function(function)
	, m_entryPoint(new EntryPoint(entryPoint))
{
}

const Type *FunctionValue::type() const
{
	auto arguments = m_function->arguments();

	std::vector<const Type *> types;

	std::transform(begin(arguments), end(arguments), std::back_inserter(types), [this](auto argument)
	{
		return argument->type(*m_entryPoint);
	});

	return FunctionType::get(types);
}

const BaseValue *FunctionValue::clone(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return this;
}

const CallableNode *FunctionValue::function() const
{
	return m_function;
}
