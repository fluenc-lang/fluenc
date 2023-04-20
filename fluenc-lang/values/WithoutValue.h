#ifndef WITHOUTVALUE_H
#define WITHOUTVALUE_H

#include "BaseValue.h"

#include "metadata/WithoutValueMetadata.h"

class WithoutValue : public BaseValueWithMetadata<WithoutValueMetadata>
{
	public:
		static const WithoutValue *instance();

		ValueId id() const override;

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint, CloneStrategy strategy) const override;
};

#endif // WITHOUTVALUE_H
