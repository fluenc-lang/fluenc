#include "string_operator_set.hpp"

#include "fragments/binary_fragment.hpp"
#include "fragments/unary_fragment.hpp"

namespace fluenc::code_generation::types
{
	fragment* string_operator_set::for_binary(const fluenc::binary_node* node) const
	{
		return new fragments::string_binary_fragment(node);
	}

	fragment* string_operator_set::for_unary(const fluenc::unary_node* node) const
	{
		return new fragments::string_unary_fragment(node);
	}
}
