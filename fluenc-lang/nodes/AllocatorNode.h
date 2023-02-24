#ifndef ALLOCATORNODE_H
#define ALLOCATORNODE_H

#include "Node.h"

class AllocatorNode : public Node
{
	public:
		AllocatorNode(const Type *type, const Node *consumer);

		static const BaseValue *alloc(const Type *type, const DefaultNodeVisitor &visitor, const EntryPoint &entryPoint);

		std::vector<DzResult> accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const override;

	private:
		const Type *m_type;
		const Node *m_consumer;
};

#endif // ALLOCATORNODE_H
