#pragma once

#include <string>
#include <vector>

#include "expression.hpp"
#include "token_info.hpp"

namespace fluenc
{
	enum class function_call_type
	{
		regular,
		tail
	};

	struct function_call_node
	{
		function_call_type type;

		std::shared_ptr<peg::Ast> ast;

		std::vector<expression_t> values;
		std::vector<std::string> names;

		expression_t consumer;
	};
}
