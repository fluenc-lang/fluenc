#ifndef EXPANDEDVALUE_H
#define EXPANDEDVALUE_H

#include "BaseValue.h"

class IteratorType;

class ExpandedValue : public BaseValue
{
	public:
		ExpandedValue(const Type *iteratorType, const EntryPoint *provider);

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint) const override;

		const EntryPoint *provider() const;

	private:
		const Type *m_iteratorType;
		const EntryPoint *m_provider;
};

#endif // EXPANDEDVALUE_H
