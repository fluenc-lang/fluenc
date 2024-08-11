#pragma once

#include "base_value.hpp"

#include "metadata/indexed_value_metadata.hpp"

namespace fluenc::code_generation::values
{
	struct indexed_value : public base_value_with_metadata<indexed_value_metadata>
	{
		indexed_value(size_t index, const base_value* subject);

		value_id id() const override;

		size_t index() const;

		const base_type* type() const override;
		const base_value* clone(const entry_point& entryPoint, clone_strategy strategy) const override;

		const base_value* subject() const;

	private:
		size_t index_;

		const base_value* subject_;
	};
}
