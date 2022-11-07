#ifndef ITERATABLENODE_H
#define ITERATABLENODE_H

#include "Node.h"

class IIteratable;

class IteratableNode : public VisitableNode<IteratableNode>
{
	friend class Emitter;

	public:
		IteratableNode(const IIteratable *iteratable);

	private:
		const IIteratable *m_iteratable;
};

#endif // ITERATABLENODE_H
