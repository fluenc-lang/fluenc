#pragma once

#include "fragment.hpp"

namespace fluenc::code_generation::fragments
{
	struct continuation_fragment : public fragment
	{
		continuation_fragment(const fragment* subject, const fragment* consumer);

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

	private:
		const fragment* subject_;
		const fragment* consumer_;
	};
}
