#include "EntryPoint.h"
#include "utility.hpp"

#include "values/FunctionValue.h"

#include "types/FunctionType.h"
#include "types/AggregateType.h"

#include "ast/function_node.hpp"

function_value::function_value(const std::vector<fluenc::function_node*> functions, const EntryPoint &entryPoint)
	: m_functions(functions)
	, m_entryPoint(new EntryPoint(entryPoint))
{
}

value_id function_value::id() const
{
	return value_id::function;
}

const Type *function_value::type() const
{
	std::vector<const Type *> functionTypes;

	std::transform(begin(m_functions), end(m_functions), back_inserter(functionTypes), [this](auto function)
	{
		std::vector<const Type *> argumentTypes;

		std::transform(begin(function->signature.arguments), end(function->signature.arguments), std::back_inserter(argumentTypes), [this](auto argument)
		{
			return arg_type(argument, *m_entryPoint);
		});

		return function_type::get(argumentTypes, function);
	});

	return aggregate_type::get(functionTypes);
}

const BaseValue *function_value::clone(const EntryPoint &entryPoint, clone_strategy strategy) const
{
	UNUSED(entryPoint);
	UNUSED(strategy);

	return this;
}

std::vector<fluenc::function_node *> function_value::functions() const
{
	return m_functions;
}
