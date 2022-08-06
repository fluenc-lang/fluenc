#ifndef ITERATORVALUE_H
#define ITERATORVALUE_H

#include "Node.h"
#include "IIteratable.h"

class EntryPoint;
class ITypeName;

class IteratorValue : public IIteratable
{
	friend class Emitter;
	friend class Analyzer;

	public:
		IteratorValue(const EntryPoint *entryPoint
			, const Node *subject
			);

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const override;

	private:
		const EntryPoint *m_entryPoint;
		const Node *m_subject;
};

#endif // ITERATORVALUE_H
