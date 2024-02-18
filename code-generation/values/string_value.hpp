#pragma once

#include "base_value.hpp"

#include "metadata/string_value_metadata.hpp"

namespace fluenc::code_generation::values
{
	struct reference_value;
	struct pre_lazy_value;

	struct string_value : public base_value_with_metadata<string_value_metadata>
	{
		string_value(const reference_value* address, const reference_value* length);

		value_id id() const override;

		const reference_value* reference() const;
		const reference_value* length() const;

		const pre_lazy_value* iterator(const entry_point& entry_point) const;

		const base_type* type() const override;

		const base_value* clone(const entry_point& entry_point, clone_strategy strategy) const override;

	private:
		const reference_value* address_;
		const reference_value* length_;
	};
}
