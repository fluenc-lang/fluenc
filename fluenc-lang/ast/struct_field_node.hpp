#pragma once

#include <optional>
#include <string>

#include "expression.hpp"
#include "type_name_node.hpp"

namespace fluenc
{
	struct struct_field_t
	{
		std::string name;
		std::optional<expression_t> default_value;
		std::optional<type_name_t> type;
	};
}
