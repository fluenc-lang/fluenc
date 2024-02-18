#include "types/StringOperatorSet.h"

#include "fragments/binary_fragment.hpp"
#include "fragments/unary_fragment.hpp"

fragment *StringOperatorSet::forBinary(const fluenc::binary_node *node) const
{
	auto result = new string_binary_fragment();
	result->binary = node;

	return result;
}

fragment *StringOperatorSet::forUnary(const fluenc::unary_node *node) const
{
	auto result = new string_unary_fragment();
	result->unary = node;

	return result;
}
