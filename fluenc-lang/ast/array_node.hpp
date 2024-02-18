#pragma once

#include <memory>
#include <vector>

#include "expression.hpp"
#include "indexed.hpp"
#include "token_info.hpp"

namespace fluenc
{
	struct array_node
	{
		const std::shared_ptr<peg::Ast> ast;

		std::vector<indexed<expression_t>> elements;

		expression_t consumer;
	};
}
