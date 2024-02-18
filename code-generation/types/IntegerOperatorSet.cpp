#include "types/IntegerOperatorSet.h"

#include "fragments/binary_fragment.hpp"
#include "fragments/unary_fragment.hpp"

fragment *IntegerOperatorSet::forBinary(const fluenc::binary_node *node) const
{
	auto result = new integer_binary_fragment();
	result->binary = node;;

	return result;
}

fragment *IntegerOperatorSet::forUnary(const fluenc::unary_node *node) const
{
	auto result = new integer_unary_fragment();
	result->unary = node;

	return result;
}
