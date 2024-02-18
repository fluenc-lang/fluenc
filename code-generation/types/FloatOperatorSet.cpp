#include "types/FloatOperatorSet.h"

#include "fragments/binary_fragment.hpp"
#include "fragments/unary_fragment.hpp"

fragment* FloatOperatorSet::forBinary(const fluenc::binary_node *node) const
{
	auto result = new float_binary_fragment();
	result->binary = node;

	return result;
}

fragment *FloatOperatorSet::forUnary(const fluenc::unary_node *node) const
{
	auto result = new float_unary_fragment();
	result->unary = node;

	return result;
}
