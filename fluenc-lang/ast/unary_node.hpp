#pragma once

#include <memory>
#include <string>

#include "expression.hpp"

namespace fluenc
{
	struct unary_node
	{
		std::shared_ptr<peg::Ast> ast;

		expression_t consumer;

		std::string op;
	};
}