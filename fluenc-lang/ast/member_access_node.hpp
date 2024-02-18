#pragma once

#include <memory>
#include <vector>

#include "expression.hpp"
#include "token_info.hpp"

namespace fluenc
{
	struct member_access_node
	{
		expression_t consumer;

		std::shared_ptr<peg::Ast> ast;
		std::vector<std::string> names;
	};
}
