#ifndef INDEXEDVALUE_H
#define INDEXEDVALUE_H

#include "BaseValue.h"

#include "metadata/IndexedValueMetadata.h"

class IndexedValue : public BaseValueWithMetadata<IndexedValueMetadata>
{
	public:
		IndexedValue(size_t index, const BaseValue *subject);

		size_t index() const;

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint, CloneStrategy strategy) const override;

		const BaseValue *subject() const;

	private:
		size_t m_index;

		const BaseValue *m_subject;
};

#endif // INDEXEDVALUE_H
