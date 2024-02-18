#pragma once

#include <vector>

#include "struct_field_node.hpp"

namespace fluenc
{
	struct struct_node
	{
		const std::shared_ptr<peg::Ast> ast;

		std::string tag;

		std::vector<struct_field_t> fields;
		std::vector<type_name_t> parent_types;
	};
}
