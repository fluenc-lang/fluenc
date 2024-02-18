#ifndef BUFFERVALUE_H
#define BUFFERVALUE_H

#include "BaseValue.h"

#include "metadata/BufferValueMetadata.h"

class BufferValue : public BaseValueWithMetadata<BufferValueMetadata>
{
	public:
        BufferValue(const reference_value *address);

                value_id id() const override;

                const reference_value *address() const;
                const reference_value *reference(const EntryPoint &entryPoint) const;
                const lazy_value *iterator(const EntryPoint &entryPoint) const;

		const Type *type() const override;
                const BaseValue *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;

	private:
                const reference_value *m_address;
};

#endif // BUFFERVALUE_H
