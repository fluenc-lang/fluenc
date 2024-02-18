#pragma once

#include "expression.hpp"
#include "fragment.hpp"
#include "indexed.hpp"
#include "token_info.hpp"

#include <memory>

namespace fluenc::code_generation::fragments
{
	struct array_sink_fragment : public fragment
	{
		array_sink_fragment(
			const std::vector<indexed<expression_t>>& elements,
			const std::shared_ptr<peg::Ast> ast,
			const fragment* consumer
		);

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

	private:
		std::vector<indexed<expression_t>> elements_;
		std::shared_ptr<peg::Ast> ast_;

		const fragment* consumer_;
	};
}
