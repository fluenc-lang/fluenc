#ifndef PLACEHOLDERVALUE_H
#define PLACEHOLDERVALUE_H

#include "BaseValue.h"

#include "metadata/PlaceholderValueMetadata.h"

class PlaceholderValue : public BaseValueWithMetadata<PlaceholderValueMetadata>
{
	public:
		static PlaceholderValue *instance();

		const Type *type() const override;

		const BaseValue *clone(const EntryPoint &entryPoint, CloneStrategy strategy) const override;
};

#endif // PLACEHOLDERVALUE_H
