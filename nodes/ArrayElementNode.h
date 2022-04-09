#ifndef ARRAYELEMENTNODE_H
#define ARRAYELEMENTNODE_H

#include <numeric>

#include "Node.h"

class Type;

class ArrayElementNode : public Node
{
	public:
		ArrayElementNode(const Type *iteratorType, const Node *next);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Type *m_iteratorType;

		const Node *m_next;
};

#endif // ARRAYELEMENTNODE_H
