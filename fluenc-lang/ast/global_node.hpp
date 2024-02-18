#pragma once

#include <memory>
#include <string>

#include "expression.hpp"
#include "token_info.hpp"

namespace fluenc
{
	struct global_node
	{
		std::shared_ptr<peg::Ast> ast;

		expression_t value;

		std::string name;
	};
}
