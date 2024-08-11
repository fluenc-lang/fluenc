#include "without_operator_set.hpp"

#include "fragments/binary_fragment.hpp"
#include "fragments/unary_fragment.hpp"

namespace fluenc::code_generation::types
{
	fragment* without_operator_set::for_binary(const binary_node* node) const
	{
		static fragments::without_binary_fragment instance;

		UNUSED(node);

		return &instance;
	}

	fragment* without_operator_set::for_unary(const unary_node* node) const
	{
		UNUSED(node);

		static fragments::without_unary_fragment instance;

		return &instance;
	}
}
