#pragma once

#include "base_value.hpp"

#include "metadata/tuple_value_metadata.hpp"

namespace fluenc::code_generation::values
{
	struct tuple_value : public base_value_with_metadata<tuple_value_metadata>
	{
		tuple_value(const std::vector<const base_value*>& values);

		value_id id() const override;

		size_t size() const;

		const base_type* type() const override;
		const base_value* clone(const entry_point& entryPoint, clone_strategy strategy) const override;

		const base_type* iteratorType() const;

		value_stack_t values() const;

	private:
		std::vector<const base_value*> values_;
	};
}
