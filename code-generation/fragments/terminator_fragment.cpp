#include "terminator_fragment.hpp"
#include "emitter.hpp"
#include "utility.hpp"

namespace fluenc::code_generation::fragments
{
	std::vector<emitter_result> terminator_fragment::build(const emitter& visitor, emitter_context context) const
	{
		UNUSED(visitor);

		return { { context.entryPoint, context.values } };
	}
}
