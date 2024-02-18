#pragma once

#include "fragment.hpp"
#include "expression.hpp"

struct junction_node// : public fragment
{
	fluenc::expression_t subject;

	// std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override;
};
