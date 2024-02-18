#pragma once

#include "fragment.hpp"
#include "value_stack.hpp"

namespace fluenc::code_generation::fragments
{
	struct pod_fragment : public fragment
	{
		pod_fragment(fragment* subject, const value_stack& output_values);

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

	private:
		fragment* subject_;

		const value_stack output_values_;
	};
}
