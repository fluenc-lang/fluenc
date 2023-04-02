#ifndef STATICNODE_H
#define STATICNODE_H

#include "Node.h"

class StaticNode : public Node
{
	public:
		StaticNode(const BaseValue *value, const Node *consumer);

		std::vector<DzResult> accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const;

	private:
		const BaseValue *m_value;
		const Node *m_consumer;
};

#endif // STATICNODE_H
