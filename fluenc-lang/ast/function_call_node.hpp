#pragma once

#include <string>
#include <vector>

#include "expression.hpp"
#include "token_info.hpp"

namespace fluenc
{
	struct function_call_node
	{
		std::shared_ptr<peg::Ast> ast;

		std::vector<expression_t> values;
		std::vector<std::string> names;

		expression_t consumer;
	};
}
