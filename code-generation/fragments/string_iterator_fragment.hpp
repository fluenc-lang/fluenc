#pragma once

#include "fragment.hpp"

namespace fluenc::code_generation
{
	namespace values
	{
		struct reference_value;
	}

	namespace fragments
	{
		struct string_iterator_fragment : public fragment
		{
			string_iterator_fragment(
				const values::reference_value* index,
				const values::reference_value* address,
				const values::reference_value* length
			);

			std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

		private:
			const values::reference_value* index_;
			const fragment* node_;

			const values::reference_value* address_;
			const values::reference_value* length_;
		};
	}
}
