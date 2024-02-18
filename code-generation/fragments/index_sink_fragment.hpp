#pragma once

#include "fragment.hpp"

namespace fluenc::code_generation::fragments
{
	struct index_sink_fragment : public fragment
	{
		index_sink_fragment(size_t index, const fragment* consumer);

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

	private:
		size_t index_;

		const fragment* consumer_;
	};
}
