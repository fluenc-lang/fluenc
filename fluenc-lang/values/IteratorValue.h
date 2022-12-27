#ifndef ITERATORVALUE_H
#define ITERATORVALUE_H

#include "Node.h"
#include "IIteratable.h"

class EntryPoint;
class ITypeName;

class IteratorValue : public IIteratable
{
	friend class Emitter;

	public:
		IteratorValue(const EntryPoint *entryPoint
			, const Node *subject
			);

		std::vector<DzResult> accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const override;

	private:
		const EntryPoint *m_entryPoint;
		const Node *m_subject;
};

#endif // ITERATORVALUE_H
