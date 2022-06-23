#include "Type.h"

#include "types/AnyType.h"

std::string Type::fullName() const
{
	return name();
}

IOperatorSet *Type::operators() const
{
	return nullptr;
}
