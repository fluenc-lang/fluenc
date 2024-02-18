#include "boolean_operator_set.hpp"

#include "fragments/binary_fragment.hpp"
#include "fragments/unary_fragment.hpp"

namespace fluenc::code_generation::types
{
	fragment* boolean_operator_set::for_binary(const binary_node* node) const
	{
		return new fragments::boolean_binary_fragment(node);
	}

	fragment* boolean_operator_set::for_unary(const unary_node* node) const
	{
		return new fragments::boolean_unary_fragment(node);
	}
}
