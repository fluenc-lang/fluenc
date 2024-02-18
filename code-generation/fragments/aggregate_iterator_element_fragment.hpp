#pragma once

#include "fragment.hpp"

namespace fluenc::code_generation
{
	struct base_value;

	namespace fragments
	{
		struct aggregate_iterator_element_fragment : public fragment
		{
			aggregate_iterator_element_fragment(const base_value* value, const fragment* next);

			std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

		private:
			const base_value* value_;
			const fragment* consumer_;
		};
	}
}
