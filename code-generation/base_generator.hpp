#pragma once

#include "base_value.hpp"
#include "emitter.hpp"

#include <vector>

struct entry_point;

namespace fluenc::code_generation
{
	namespace generators
	{
		enum class generation_mode
		{
			regular = 0,
			dry_run
		};

		struct base_generator
		{
			virtual std::vector<emitter_result> generate(
				const emitter& visitor,
				emitter_context context,
				generation_mode mode
			) const = 0;

			virtual const base_generator* clone(const entry_point& entry_point, clone_strategy strategy) const = 0;

			virtual const base_type* type() const = 0;
		};
	}
}
