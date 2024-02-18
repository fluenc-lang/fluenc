#ifndef EXPANDEDLAZYVALUE_H
#define EXPANDEDLAZYVALUE_H

#include "BaseValue.h"

#include "metadata/ExpandedLazyValueMetadata.h"

class lazy_value;

class ExpandedLazyValue : public BaseValueWithMetadata<ExpandedLazyValueMetadata>
{
	public:
        ExpandedLazyValue(const lazy_value *subject);

                value_id id() const override;

		const Type *type() const override;
                const BaseValue *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;
                const lazy_value *subject() const;

	private:
                const lazy_value *m_subject;
};

#endif // EXPANDEDLAZYVALUE_H
