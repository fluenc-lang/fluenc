#include "WithoutOperatorSet.h"

#include "fragments/binary_fragment.hpp"
#include "fragments/unary_fragment.hpp"

fragment* WithoutOperatorSet::forBinary(const fluenc::binary_node *node) const
{
	auto result = new without_binary_fragment();
	result->binary = node;

	return result;
}

fragment* WithoutOperatorSet::forUnary(const fluenc::unary_node *node) const
{
	auto result = new without_unary_fragment();
	result->unary = node;

	return result;
}
