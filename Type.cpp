#include "Type.h"

#include "types/AnyType.h"

std::string Type::fullName() const
{
	return name();
}

int8_t Type::compatibility(const Type *type, const EntryPoint &entryPoint) const
{
	if (is(type, entryPoint))
	{
		return 0;
	}

	if (dynamic_cast<const AnyType *>(type))
	{
		return 1;
	}

	return -1;
}
