#pragma once

#include <vector>

struct entry_point;

namespace fluenc::code_generation
{
	struct emitter;
	struct emitter_context;
	struct emitter_result;

	struct fragment
	{
		virtual std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const = 0;
	};
}
