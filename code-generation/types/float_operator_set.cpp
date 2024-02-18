#include "float_operator_set.hpp"

#include "fragments/binary_fragment.hpp"
#include "fragments/unary_fragment.hpp"

namespace fluenc::code_generation::types
{
	fragment* float_operator_set::for_binary(const binary_node* node) const
	{
		return new fragments::float_binary_fragment(node);
	}

	fragment* float_operator_set::for_unary(const unary_node* node) const
	{
		return new fragments::float_unary_fragment(node);
	}
}
