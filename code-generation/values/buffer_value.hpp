#pragma once

#include "base_value.hpp"

#include "metadata/buffer_value_metadata.hpp"

namespace fluenc::code_generation::values
{
	struct buffer_value : public base_value_with_metadata<buffer_value_metadata>
	{
		buffer_value(const reference_value* address);

		value_id id() const override;

		const reference_value* address() const;
		const reference_value* reference(const entry_point& entryPoint) const;
		const pre_lazy_value* iterator(const entry_point& entryPoint) const;

		const base_type* type() const override;
		const base_value* clone(const entry_point& entryPoint, clone_strategy strategy) const override;

	private:
		const reference_value* address_;
	};
}
