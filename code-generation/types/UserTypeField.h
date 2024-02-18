#pragma once

#include <string>

class Type;

struct user_type_field
{
	size_t index;

	std::string name;

	const Type *type;
};
