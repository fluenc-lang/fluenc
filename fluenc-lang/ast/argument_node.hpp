#pragma once

#include "type_name_node.hpp"

#include <variant>

namespace fluenc
{
	struct default_argument;
	struct tuple_argument;

	using argument_t = std::variant<const default_argument*, const tuple_argument*>;

	struct default_argument
	{
		std::string name;

		type_name_t type;
	};

	struct tuple_argument
	{
		std::vector<argument_t> arguments;
	};
}
