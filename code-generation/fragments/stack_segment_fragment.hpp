#pragma once

#include <vector>

#include "expression.hpp"
#include "fragment.hpp"

namespace fluenc::code_generation::fragments
{
	struct stack_segment_fragment : public fragment
	{
		stack_segment_fragment(const std::vector<fluenc::expression_t>& values, fragment* call, fluenc::expression_t consumer);

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

	private:
		std::vector<fluenc::expression_t> values_;

		fragment* call_;
		fluenc::expression_t consumer_;
	};
}
