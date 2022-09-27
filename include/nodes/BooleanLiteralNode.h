#ifndef BOOLEANLITERALNODE_H
#define BOOLEANLITERALNODE_H

#include "Node.h"

class ScalarValue;

class BooleanLiteralNode : public VisitableNode<BooleanLiteralNode>
{
	friend class Emitter;

	public:
		BooleanLiteralNode(const Node *consumer, const std::string &value);

	private:
		const ScalarValue *resolveValue(const EntryPoint &entryPoint) const;

		const Node *m_consumer;

		std::string m_value;
};

#endif // BOOLEANLITERALNODE_H
