#ifndef ITERATABLENODE_H
#define ITERATABLENODE_H

#include "Node.h"

class IIteratable;

class IteratableNode : public Node
{
	public:
		IteratableNode(const IIteratable *iteratable);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const IIteratable *m_iteratable;
};

#endif // ITERATABLENODE_H
