#pragma once

#include <vector>

#include "base_value.hpp"
#include "named_value.hpp"

#include "metadata/user_type_value_metadata.hpp"

namespace fluenc::code_generation
{
	namespace types
	{
		struct structure_type;
	}

	namespace values
	{
		struct user_type_value : public base_value_with_metadata<user_type_value_metadata>
		{
			user_type_value(const types::structure_type* type, const std::vector<const named_value*>& values);

			value_id id() const override;

			const base_type* type() const override;
			const base_value* clone(const entry_point& entryPoint, clone_strategy strategy) const override;

			const types::structure_type* prototype() const;

			std::vector<const named_value*> fields() const;

		private:
			const types::structure_type* type_;

			std::vector<const named_value*> values_;
		};
	}
}
