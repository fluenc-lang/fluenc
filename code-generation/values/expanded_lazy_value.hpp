#pragma once

#include "base_value.hpp"

#include "metadata/expanded_lazy_value_metadata.hpp"

namespace fluenc::code_generation::values
{
	struct pre_lazy_value;

	struct expanded_lazy_value : public base_value_with_metadata<expanded_lazy_value_metadata>
	{
		expanded_lazy_value(const pre_lazy_value* subject);

		value_id id() const override;

		const base_type* type() const override;
		const base_value* clone(const entry_point& entryPoint, clone_strategy strategy) const override;
		const pre_lazy_value* subject() const;

	private:
		const pre_lazy_value* subject_;
	};
}
