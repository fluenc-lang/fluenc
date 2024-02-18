#pragma once

#include "BaseValue.h"

#include "metadata/WithoutValueMetadata.h"

class without_value : public BaseValueWithMetadata<WithoutValueMetadata>
{
	public:
		static const without_value *instance();

                value_id id() const override;

		const Type *type() const override;
                const BaseValue *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;
};
