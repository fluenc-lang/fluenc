#ifndef TUPLEVALUE_H
#define TUPLEVALUE_H

#include <sstream>
#include <numeric>

#include "BaseValue.h"
#include "Stack.h"

#include "metadata/TupleValueMetadata.h"

class TupleValue : public BaseValueWithMetadata<TupleValueMetadata>
{
	public:
		TupleValue(const std::vector<const BaseValue *> &values);

		size_t size() const;

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint) const override;

		const Type *iteratorType() const;

		Stack values() const;

	private:
		std::vector<const BaseValue *> m_values;
};

#endif // TUPLEVALUE_H
