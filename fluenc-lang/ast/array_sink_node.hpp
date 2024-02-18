#pragma once

#include <memory>

#include "expression.hpp"
#include "token_info.hpp"

namespace fluenc
{
	struct array_sink_node
	{
		size_t size;

		const std::shared_ptr<peg::Ast> ast;

		expression_t consumer;
		expression_t first_value;
	};
}
