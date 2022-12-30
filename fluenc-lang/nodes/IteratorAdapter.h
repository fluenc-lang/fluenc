#ifndef ITERATORADAPTER_H
#define ITERATORADAPTER_H

#include "Node.h"

class IIteratable;

class IteratorAdapter : public Node
{
	public:
		IteratorAdapter(const IIteratable *subject);

		std::vector<DzResult> accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const;

	private:
		const IIteratable *m_subject;
};


#endif // ITERATORADAPTER_H
