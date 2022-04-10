#ifndef ITERATORVALUE_H
#define ITERATORVALUE_H

#include "BaseValue.h"
#include "Node.h"
#include "IIteratable.h"

class EntryPoint;
class ITypeName;

class IteratorValue : public IIteratable
{
	public:
		IteratorValue(const EntryPoint *entryPoint
			, const Node *subject
			);

		std::vector<DzResult> build(const EntryPoint &entryPoint) const override;

	private:
		const EntryPoint *m_entryPoint;
		const Node *m_subject;
};

#endif // ITERATORVALUE_H
