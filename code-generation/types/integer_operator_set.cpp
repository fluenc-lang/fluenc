#include "integer_operator_set.hpp"

#include "fragments/binary_fragment.hpp"
#include "fragments/unary_fragment.hpp"

namespace fluenc::code_generation::types
{
	fragment* integer_operator_set::for_binary(const binary_node* node) const
	{
		return new fragments::integer_binary_fragment(node);
	}

	fragment* integer_operator_set::for_unary(const unary_node* node) const
	{
		return new fragments::integer_unary_fragment(node);
	}
}
