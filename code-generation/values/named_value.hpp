#pragma once

#include "base_value.hpp"

#include "metadata/named_value_metadata.hpp"

namespace fluenc::code_generation::values
{
	struct named_value : public base_value_with_metadata<named_value_metadata>
	{
		named_value(const std::string& name, const base_value* value);

		value_id id() const override;

		std::string name() const;

		const base_value* value() const;

		const base_type* type() const override;
		const base_value* clone(const entry_point& entryPoint, clone_strategy strategy) const override;

	private:
		std::string name_;

		const base_value* value_;
	};
}
