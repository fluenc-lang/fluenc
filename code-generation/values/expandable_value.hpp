#pragma once

#include "base_value.hpp"
#include "value_stack.hpp"

#include "metadata/expandable_value_metadata.hpp"

namespace fluenc::code_generation
{
	namespace types
	{
		struct iterator_type;
		struct expanded_type;
	}

	struct entry_point;
	struct fragment;

	namespace values
	{
		struct expandable_value : public base_value_with_metadata<expandable_value_metadata>
		{
			expandable_value(
				bool is_array,
				const base_type* iterator_type,
				const entry_point& provider,
				const fragment* chain,
				const value_stack& values
			);

			value_id id() const override;

			bool isArray() const;

			const base_type* type() const override;
			const base_value* clone(const entry_point& entryPoint, clone_strategy strategy) const override;

			const entry_point* provider() const;
			const fragment* chain() const;
			const types::expanded_type* expandedType() const;
			const value_stack values() const;

		private:
			bool is_array_;

			const base_type* iterator_type_;
			const entry_point* provider_;
			const fragment* chain_;
			const value_stack values_;
		};
	}
}
