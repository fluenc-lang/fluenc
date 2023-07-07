#ifndef BUFFERVALUE_H
#define BUFFERVALUE_H

#include "BaseValue.h"

#include "metadata/BufferValueMetadata.h"

class BufferValue : public BaseValueWithMetadata<BufferValueMetadata>
{
	public:
		BufferValue(const ReferenceValue *address);

		ValueId id() const override;

		const ReferenceValue *address() const;
		const ReferenceValue *reference(const EntryPoint &entryPoint) const;
		const LazyValue *iterator(const EntryPoint &entryPoint) const;

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint, CloneStrategy strategy) const override;

	private:
		const ReferenceValue *m_address;
};

#endif // BUFFERVALUE_H
