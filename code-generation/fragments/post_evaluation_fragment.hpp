#pragma once

#include "fragment.hpp"

namespace fluenc::code_generation::fragments
{
	struct post_evaluation_fragment : public fragment
	{
		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;
	};
}
