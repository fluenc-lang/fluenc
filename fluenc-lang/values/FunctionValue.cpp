#include "EntryPoint.h"
#include "DzBaseArgument.h"
#include "Utility.h"
#include "CallableNode.h"

#include "values/FunctionValue.h"

#include "types/FunctionType.h"
#include "types/AggregateType.h"

FunctionValue::FunctionValue(const std::vector<const CallableNode *> functions, const EntryPoint &entryPoint)
	: m_functions(functions)
	, m_entryPoint(new EntryPoint(entryPoint))
{
}

const Type *FunctionValue::type() const
{
	std::vector<const Type *> functionTypes;

	std::transform(begin(m_functions), end(m_functions), back_inserter(functionTypes), [this](auto function)
	{
		auto arguments = function->arguments();

		std::vector<const Type *> argumentTypes;

		std::transform(begin(arguments), end(arguments), std::back_inserter(argumentTypes), [this](auto argument)
		{
			return argument->type(*m_entryPoint);
		});

		return FunctionType::get(argumentTypes, function);
	});

	return AggregateType::get(functionTypes);
}

const BaseValue *FunctionValue::clone(const EntryPoint &entryPoint, CloneStrategy strategy) const
{
	UNUSED(entryPoint);
	UNUSED(strategy);

	return this;
}

std::vector<const CallableNode *> FunctionValue::functions() const
{
	return m_functions;
}
