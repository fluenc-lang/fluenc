#ifndef STRINGVALUE_H
#define STRINGVALUE_H

#include "BaseValue.h"

#include "metadata/StringValueMetadata.h"

class Node;
class reference_value;
class lazy_value;

class StringValue : public BaseValueWithMetadata<StringValueMetadata>
{
	public:
        StringValue(const reference_value *address, const reference_value *length);

                value_id id() const override;

                const reference_value *reference() const;
                const reference_value *length() const;

                const lazy_value *iterator(const EntryPoint &entryPoint) const;

		const Type *type() const override;

                const BaseValue *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;

	private:
                const reference_value *m_address;
                const reference_value *m_length;
};

#endif // STRINGVALUE_H
