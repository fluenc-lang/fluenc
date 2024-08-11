#pragma once

#include "fragment.hpp"

namespace fluenc::code_generation::fragments
{
	struct reference_sink_fragment : public fragment
	{
		reference_sink_fragment(const fragment* consumer);

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

	private:
		const fragment* consumer_;
	};
}
