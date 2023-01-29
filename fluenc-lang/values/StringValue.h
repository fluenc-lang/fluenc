#ifndef STRINGVALUE_H
#define STRINGVALUE_H

#include "BaseValue.h"

#include "metadata/StringValueMetadata.h"

class Node;
class ReferenceValue;
class LazyValue;

class StringValue : public BaseValueWithMetadata<StringValueMetadata>
{
	public:
		StringValue(const ReferenceValue *address, size_t id, size_t length);

		const ReferenceValue *reference() const;
		const LazyValue *iterator() const;

		const Type *type() const override;

		const BaseValue *clone(const EntryPoint &entryPoint, CloneStrategy strategy) const override;
		const BaseValue *forward(size_t id) const override;

	private:
		const ReferenceValue *m_address;

		size_t m_id;
		size_t m_length;
};

#endif // STRINGVALUE_H
