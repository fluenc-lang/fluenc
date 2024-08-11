#pragma once

#include "fragment.hpp"

namespace fluenc::code_generation::fragments
{
	struct terminator_fragment : public fragment
	{
		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

		static const fragment* instance()
		{
			static terminator_fragment instance;

			return &instance;
		}
	};
}
