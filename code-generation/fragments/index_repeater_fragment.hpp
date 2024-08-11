#pragma once

#include "fragment.hpp"

namespace fluenc::code_generation::fragments
{
	struct index_repeater_fragment : public fragment
	{
		index_repeater_fragment(const fragment* iteratable);

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

	private:
		const fragment* subject_;
	};
}
