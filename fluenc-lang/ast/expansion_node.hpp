#pragma once

#include <memory>

#include "expression.hpp"
#include "token_info.hpp"

namespace fluenc
{
	struct expansion_node
	{
		expression_t consumer;

		std::shared_ptr<peg::Ast> ast;
	};
}
