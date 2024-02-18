#pragma once

#include <memory>

#include "fragment.hpp"
#include "token_info.hpp"

namespace fluenc::code_generation
{
	struct base_type;

	namespace fragments
	{
		struct array_iterator_fragment : public fragment
		{
			array_iterator_fragment(
				const std::shared_ptr<peg::Ast>& ast,
				const base_type* type,
				const std::vector<emitter_result>& values,
				size_t size
			);

			std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

		private:
			std::shared_ptr<peg::Ast> ast_;
			std::vector<emitter_result> values_;

			const base_type* type_;

			size_t size_;
		};
	}
}
