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

		std::vector<DzResult<BaseValue>> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;

	private:
		const Type *m_arrayType;

		const Node *m_node;
		const Node *m_next;
};

#endif // ARRAYELEMENTNODE_H
