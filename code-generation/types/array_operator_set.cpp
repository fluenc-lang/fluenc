#include "array_operator_set.hpp"

#include "fragments/binary_fragment.hpp"
#include "fragments/unary_fragment.hpp"

namespace fluenc::code_generation::types
{
	fragment* array_operator_set::for_binary(const binary_node* node) const
	{
		return new fragments::array_binary_fragment(node);
	}

	fragment* array_operator_set::for_unary(const unary_node* node) const
	{
		return new fragments::array_unary_fragment(node);
	}
}
