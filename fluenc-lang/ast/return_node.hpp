#pragma once

#include "expression.hpp"

#include <cstdint>
#include <optional>

namespace fluenc
{
	struct return_node
	{
		std::optional<uint64_t> iterator;
		std::optional<expression_t> chained;

		expression_t consumer;
	};
}
