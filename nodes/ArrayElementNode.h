#ifndef ARRAYELEMENTNODE_H
#define ARRAYELEMENTNODE_H

#include <numeric>

#include "Node.h"

class Type;

class ArrayElementNode : public Node
{
	public:
		ArrayElementNode(const Type *arrayType, const Node *next);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Type *m_arrayType;

		const Node *m_next;
};

#endif // ARRAYELEMENTNODE_H
