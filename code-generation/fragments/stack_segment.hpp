#pragma once

#include <vector>

#include "expression.hpp"

struct fragment;

struct stack_segment_node
{
	std::vector<fluenc::expression_t> values;

	fragment* call;
	fluenc::expression_t consumer;
};
