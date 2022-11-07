#include "FunctionTypeName.h"

#include "types/FunctionType.h"

FunctionTypeName::FunctionTypeName(const std::vector<const ITypeName *> &types)
	: m_types(types)
{
}

Type *FunctionTypeName::resolve(const EntryPoint &entryPoint) const
{
	std::vector<const Type *> types;

	std::transform(begin(m_types), end(m_types), std::back_inserter(types), [&](auto type)
	{
		return type->resolve(entryPoint);
	});

	return new FunctionType(types, nullptr);
}
