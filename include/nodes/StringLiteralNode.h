#ifndef STRINGLITERALNODE_H
#define STRINGLITERALNODE_H

#include "Node.h"

class StringLiteralNode : public VisitableNode<StringLiteralNode>
{
	friend class Emitter;
	friend class Analyzer;

	public:
		StringLiteralNode(const Node *consumer, const std::string &value);

	private:
		const Node *m_consumer;

		std::string m_value;
};

#endif // STRINGLITERALNODE_H
