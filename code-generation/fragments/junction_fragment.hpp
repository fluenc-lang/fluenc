#pragma once

#include "fragment.hpp"
#include "expression.hpp"

namespace fluenc::code_generation::fragments
{
	struct junction_fragment : public fragment
	{
		junction_fragment(const fluenc::expression_t& subject);

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

	private:
		fluenc::expression_t subject_;
	};
}
