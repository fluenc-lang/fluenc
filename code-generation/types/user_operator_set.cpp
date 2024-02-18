#include "user_operator_set.hpp"

#include "ast/binary_node.hpp"
#include "ast/unary_node.hpp"

#include "fragments/binary_fragment.hpp"
#include "fragments/unary_fragment.hpp"

namespace fluenc::code_generation::types
{
	fragment* user_operator_set::for_binary(const binary_node* node) const
	{
		return new fragments::user_binary_fragment(node);
	}

	fragment* user_operator_set::for_unary(const unary_node* node) const
	{
		return new fragments::user_unary_fragment(node);
	}
}
