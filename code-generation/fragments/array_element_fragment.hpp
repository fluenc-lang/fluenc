#pragma once

#include <memory>

#include "fragment.hpp"
#include "token_info.hpp"

namespace fluenc::code_generation
{
	struct base_type;

	namespace fragments
	{
		class array_element_fragment : public fragment
		{
			friend struct emitter;

		public:
			array_element_fragment(
				const std::shared_ptr<peg::Ast>& ast,
				const base_type* array_type,
				const fragment* node,
				const fragment* next
			);

			std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

		private:
			const std::shared_ptr<peg::Ast> ast_;

			const base_type* array_type_;
			const fragment* node_;
			const fragment* next_;
		};
	}
}
