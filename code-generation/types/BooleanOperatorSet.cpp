#include "types/BooleanOperatorSet.h"

#include "fragments/binary_fragment.hpp"
#include "fragments/unary_fragment.hpp"

fragment *BooleanOperatorSet::forBinary(const fluenc::binary_node *node) const
{
	auto result = new boolean_binary_fragment();
	result->binary = node;

	return result;
}

fragment *BooleanOperatorSet::forUnary(const fluenc::unary_node *node) const
{
	auto result = new boolean_unary_fragment();
	result->unary = node;

	return result;
}
