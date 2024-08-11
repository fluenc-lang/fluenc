#pragma once

#include "fragment.hpp"

namespace fluenc::code_generation
{
	struct base_type;

	namespace values
	{
		struct reference_value;
	}

	namespace fragments
	{
		struct array_continuation_fragment : public fragment
		{
			array_continuation_fragment(
				const values::reference_value* index,
				const fragment* node,
				const base_type* iterator_type
			);

			std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

		private:
			const values::reference_value* index_;

			const fragment* node_;
			const base_type* iterator_type_;
		};
	}
}
