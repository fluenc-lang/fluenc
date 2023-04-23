#ifndef EXPANDEDLAZYVALUE_H
#define EXPANDEDLAZYVALUE_H

#include "BaseValue.h"

#include "metadata/ExpandedLazyValueMetadata.h"

class LazyValue;

class ExpandedLazyValue : public BaseValueWithMetadata<ExpandedLazyValueMetadata>
{
	public:
		ExpandedLazyValue(const LazyValue *subject);

		ValueId id() const override;

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint, CloneStrategy strategy) const override;
		const LazyValue *subject() const;

	private:
		const LazyValue *m_subject;
};

#endif // EXPANDEDLAZYVALUE_H
