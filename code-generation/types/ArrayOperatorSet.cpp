#include "ArrayOperatorSet.h"

#include "fragments/binary_fragment.hpp"
#include "fragments/unary_fragment.hpp"

fragment *ArrayOperatorSet::forBinary(const fluenc::binary_node *node) const
{
	auto result = new array_binary_fragment();
	result->binary = node;

	return result;
}

fragment *ArrayOperatorSet::forUnary(const fluenc::unary_node *node) const
{
	auto result = new array_unary_fragment();
	result->unary = node;

	return result;
}
