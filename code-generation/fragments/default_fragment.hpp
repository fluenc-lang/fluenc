#pragma once

#include "fragment.hpp"

namespace fluenc::code_generation
{
	struct base_type;

	namespace fragments
	{
		struct default_fragment : public fragment
		{
			default_fragment(const base_type* type, const fragment* consumer);

			std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

		private:
			const base_type* type_;
			const fragment* consumer_;
		};
	}
}
