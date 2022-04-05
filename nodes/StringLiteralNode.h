#ifndef STRINGLITERALNODE_H
#define STRINGLITERALNODE_H

#include "Node.h"

class StringLiteralNode : public Node
{
	public:
		StringLiteralNode(Node *consumer, const std::string &value);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		Node *m_consumer;

		std::string m_value;
};

#endif // STRINGLITERALNODE_H
