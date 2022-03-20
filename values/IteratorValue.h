#ifndef ITERATORVALUE_H
#define ITERATORVALUE_H

#include "BaseValue.h"
#include "DzValue.h"

class EntryPoint;

class IteratorValue : public BaseValue
{
	public:
		IteratorValue(const DzValue *iterator, const Stack &values);

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint) const;

	private:
		const DzValue *m_iterator;

		Stack m_values;
};

#endif // ITERATORVALUE_H
