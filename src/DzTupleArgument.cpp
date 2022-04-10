#include "DzTupleArgument.h"

#include "types/TupleType.h"

DzTupleArgument::DzTupleArgument(const std::vector<DzBaseArgument *> &arguments)
	: m_arguments(arguments)
{
}

std::vector<DzBaseArgument *> DzTupleArgument::arguments() const
{
	return m_arguments;
}

Type *DzTupleArgument::type(const EntryPoint &entryPoint) const
{
	std::vector<const Type *> types;

	std::transform(rbegin(m_arguments), rend(m_arguments), std::back_inserter(types), [&](auto arguments)
	{
		return arguments->type(entryPoint);
	});

	return TupleType::get(nullptr, types);
}
