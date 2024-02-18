#pragma once

#include <string>

class BaseValue;
class Type;

struct structure_field_t
{
	std::string name;

	const BaseValue *default_value;
	const Type *type;
};
