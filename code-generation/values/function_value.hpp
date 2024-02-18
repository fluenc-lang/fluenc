#pragma once

#include <vector>

#include "base_value.hpp"

#include "metadata/function_value_metadata.hpp"

namespace fluenc
{
	struct function_node;

	namespace code_generation
	{
		struct entry_point;

		namespace values
		{
			struct function_value : public base_value_with_metadata<function_value_metadata>
			{
				function_value(const std::vector<function_node*> functions, const entry_point& entryPoint);

				value_id id() const override;

				const base_type* type() const override;
				const base_value* clone(const entry_point& entryPoint, clone_strategy strategy) const override;

				std::vector<function_node*> functions() const;

			private:
				std::vector<function_node*> functions_;

				const entry_point* entry_point_;
			};
		}
	}
}
