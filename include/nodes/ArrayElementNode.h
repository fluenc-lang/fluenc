#ifndef ARRAYELEMENTNODE_H
#define ARRAYELEMENTNODE_H

#include <numeric>

#include "Node.h"

class Type;

class ArrayElementNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		ArrayElementNode(const Type *arrayType, const Node *node, const Node *next);

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, DefaultVisitorContext context) const override;

	private:
		const Type *m_arrayType;

		const Node *m_node;
		const Node *m_next;
};

#endif // ARRAYELEMENTNODE_H
