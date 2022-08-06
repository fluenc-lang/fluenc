#ifndef ITERATABLENODE_H
#define ITERATABLENODE_H

#include "Node.h"

class IIteratable;

class IteratableNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		IteratableNode(const IIteratable *iteratable);

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const override;

	private:
		const IIteratable *m_iteratable;
};

#endif // ITERATABLENODE_H
