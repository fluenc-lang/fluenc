#ifndef BOOLEANLITERALNODE_H
#define BOOLEANLITERALNODE_H

#include "Node.h"

class ScalarValue;

class BooleanLiteralNode : public Node
{
	public:
		BooleanLiteralNode(Node *consumer, const std::string &value);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const ScalarValue *resolveValue(const EntryPoint &entryPoint) const;

		Node *m_consumer;

		std::string m_value;
};

#endif // BOOLEANLITERALNODE_H
