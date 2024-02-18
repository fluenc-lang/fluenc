#pragma once

#include "base_value.hpp"

namespace fluenc::code_generation::values
{
	struct forwarded_value : public base_value
	{
		forwarded_value(const base_value* subject);

		value_id id() const override;

		const base_value* subject() const;

		const base_type* type() const override;
		const base_value* clone(const entry_point& entryPoint, clone_strategy strategy) const override;
		const value_metadata& metadata() const override;

	private:
		const base_value* subject_;
	};
}
