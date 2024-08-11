#pragma once

#include <variant>
#include <vector>

#include "expression.hpp"
#include "type_name_node.hpp"

namespace fluenc
{
	struct default_prototype
	{
		type_name_t type;
	};

	struct with_prototype
	{
	};

	using prototype_t = std::variant<default_prototype, with_prototype>;

	struct instantiation_node
	{
		const expression_t consumer;

		prototype_t prototype;

		std::shared_ptr<peg::Ast> ast;
		std::vector<std::string> fields;
	};
}
