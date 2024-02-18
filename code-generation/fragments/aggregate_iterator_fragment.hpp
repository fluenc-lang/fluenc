#pragma once

#include "expression.hpp"
#include "fragment.hpp"

namespace fluenc::code_generation
{
	namespace values
	{
		struct reference_value;
	}

	struct base_type;
	struct base_value;

	namespace fragments
	{
		struct aggregate_iterator_fragment : public fragment
		{
			aggregate_iterator_fragment(
				const values::reference_value* index,
				const expression_t& node,
				const base_type* array_type,
				const std::vector<const base_value*>& iteratables
			);

			std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

		private:
			const values::reference_value* index_;
			const expression_t node_;
			const base_type* array_type_;

			std::vector<const base_value*> values_;
		};
	}
}
