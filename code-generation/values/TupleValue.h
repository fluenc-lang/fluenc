#pragma once

#include <sstream>
#include <numeric>

#include "BaseValue.h"
#include "Stack.h"

#include "metadata/TupleValueMetadata.h"

class tuple_value : public BaseValueWithMetadata<TupleValueMetadata>
{
	public:
		tuple_value(const std::vector<const BaseValue *> &values);

                value_id id() const override;

		size_t size() const;

		const Type *type() const override;
                const BaseValue *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;

		const Type *iteratorType() const;

		Stack values() const;

	private:
		std::vector<const BaseValue *> m_values;
};
