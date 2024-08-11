#pragma once

#include <memory>

#include "expression.hpp"
#include "token_info.hpp"

namespace fluenc
{
	struct conditional_node
	{
		const std::shared_ptr<peg::Ast> ast;

		expression_t if_true;
		expression_t if_false;
	};
}
