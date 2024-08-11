#pragma once

#include "base_value.hpp"

#include "metadata/placeholder_value_metadata.hpp"

namespace fluenc::code_generation::values
{
	struct placeholder_value : public base_value_with_metadata<placeholder_value_metadata>
	{
		static placeholder_value* instance();

		value_id id() const override;

		const base_type* type() const override;

		const base_value* clone(const entry_point& entryPoint, clone_strategy strategy) const override;
	};
}
