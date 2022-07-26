#ifndef ARRAYELEMENTNODE_H
#define ARRAYELEMENTNODE_H

#include <numeric>

#include "Node.h"

class Type;

class ArrayElementNode : public Node
{
	friend class Emitter;

	public:
		ArrayElementNode(const Type *arrayType, const Node *node, const Node *next);

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Type *m_arrayType;

		const Node *m_node;
		const Node *m_next;
};

#endif // ARRAYELEMENTNODE_H
