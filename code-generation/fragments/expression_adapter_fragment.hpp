#pragma once

#include "expression.hpp"
#include "fragment.hpp"

namespace fluenc::code_generation::fragments
{
	struct expression_adapter_fragment : public fragment
	{
		expression_adapter_fragment(const expression_t& subject);

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

	private:
		expression_t subject_;
	};
}
