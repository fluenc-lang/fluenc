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
		StringValue(const ReferenceValue *address, const ReferenceValue *length);

		ValueId id() const override;

		const ReferenceValue *reference() const;
		const LazyValue *iterator(const EntryPoint &entryPoint) const;

		const Type *type() const override;

		const BaseValue *clone(const EntryPoint &entryPoint, CloneStrategy strategy) const override;

	private:
		const ReferenceValue *m_address;
		const ReferenceValue *m_length;
};

#endif // STRINGVALUE_H
