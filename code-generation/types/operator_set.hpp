#pragma once

namespace fluenc
{
	struct binary_node;
	struct unary_node;

	namespace code_generation
	{
		struct fragment;

		struct operator_set
		{
			virtual fragment* for_binary(const binary_node* node) const = 0;
			virtual fragment* for_unary(const unary_node* node) const = 0;
		};
	}
}
