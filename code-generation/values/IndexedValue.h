#pragma once

#include "BaseValue.h"

#include "metadata/IndexedValueMetadata.h"

class indexed_value : public BaseValueWithMetadata<IndexedValueMetadata>
{
	public:
        indexed_value(size_t index, const BaseValue *subject);

		value_id id() const override;

		size_t index() const;

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;

		const BaseValue *subject() const;

	private:
		size_t m_index;

		const BaseValue *m_subject;
};
