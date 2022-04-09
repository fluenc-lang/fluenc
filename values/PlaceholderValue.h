#ifndef PLACEHOLDERVALUE_H
#define PLACEHOLDERVALUE_H

#include "BaseValue.h"

class PlaceholderValue : public BaseValue
{
	public:
		static PlaceholderValue *instance();

		const Type *type() const override;

		const BaseValue *clone(const EntryPoint &entryPoint) const override;
};

#endif // PLACEHOLDERVALUE_H
