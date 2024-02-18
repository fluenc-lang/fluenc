#pragma once

#include "BaseValue.h"

#include "metadata/PlaceholderValueMetadata.h"

class placeholder_value : public BaseValueWithMetadata<PlaceholderValueMetadata>
{
	public:
		static placeholder_value *instance();

		value_id id() const override;

		const Type *type() const override;

		const BaseValue *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;
};
