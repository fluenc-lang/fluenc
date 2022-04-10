#ifndef WITHOUTVALUE_H
#define WITHOUTVALUE_H

#include "BaseValue.h"

class WithoutValue : public BaseValue
{
	public:
		static const WithoutValue *instance();

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint) const override;
};

#endif // WITHOUTVALUE_H
