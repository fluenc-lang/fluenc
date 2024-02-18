#pragma once

#include "base_value.hpp"
#include "value_stack.hpp"

#include "metadata/expanded_value_metadata.hpp"

namespace fluenc::code_generation
{
	struct fragment;

	namespace types
	{
		struct iterator_type;
	}

	namespace values
	{
		struct expanded_value : public base_value_with_metadata<expanded_value_metadata>
		{
			expanded_value(
				bool isArray,
				const base_type* iterator_type,
				const entry_point& provider,
				const fragment* node,
				const std::vector<const expanded_value*>& next,
				const value_stack& values
			);

			value_id id() const override;

			bool is_array() const;

			const base_type* type() const override;
			const base_value* clone(const entry_point& entryPoint, clone_strategy strategy) const override;

			const entry_point* provider() const;
			const fragment* node() const;

			std::vector<const expanded_value*> next() const;

			const value_stack& values() const;

		private:
			bool is_array_;

			const base_type* iterator_type_;
			const entry_point* provider_;
			const fragment* node_;
			const fragment* chain_;

			std::vector<const expanded_value*> next_;

			value_stack values_;
		};
	}
}
