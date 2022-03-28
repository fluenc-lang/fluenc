#ifndef ITERATORVALUE_H
#define ITERATORVALUE_H

#include "BaseValue.h"
#include "DzValue.h"
#include "IIteratable.h"

class EntryPoint;
class ITypeName;

class IteratorValue : public IIteratable
{
	public:
		IteratorValue(const EntryPoint *entryPoint
			, const DzValue *subject
			);

		std::vector<DzResult> build(const EntryPoint &entryPoint) const override;

	private:
		const EntryPoint *m_entryPoint;
		const DzValue *m_subject;
};

#endif // ITERATORVALUE_H
