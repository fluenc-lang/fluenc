#pragma once

namespace fluenc
{
	struct binary_node;
	struct unary_node;
}

struct fragment;

class IOperatorSet
{
	public:
		virtual fragment* forBinary(const fluenc::binary_node *node) const = 0;
		virtual fragment* forUnary(const fluenc::unary_node *node) const = 0;
};
