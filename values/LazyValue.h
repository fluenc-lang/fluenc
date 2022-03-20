#ifndef LAZYVALUE_H
#define LAZYVALUE_H

#include "BaseValue.h"
#include "DzValue.h"

class EntryPoint;
class ITypeName;

// TODO Create common interface (IIteratable?) for LazyValue and ArrayValue
class LazyValue : public BaseValue
{
	public:
		LazyValue(const Stack &values
			, const EntryPoint &entryPoint
			, const Type *iteratorType
			, const DzValue *subject
			);

		std::vector<DzResult> build(const EntryPoint &entryPoint, const Stack &values) const;

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint) const override;

	private:
		const Stack m_values;

		const EntryPoint *m_entryPoint;
		const Type *m_iteratorType;
		const DzValue *m_subject;
};

#endif // LAZYVALUE_H
