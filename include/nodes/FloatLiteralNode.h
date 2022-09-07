#ifndef FLOATLITERALNODE_H
#define FLOATLITERALNODE_H

#include "Node.h"

class ITypeName;

class FloatLiteralNode : public VisitableNode<FloatLiteralNode>
{
	friend class Emitter;
	friend class Analyzer;

	public:
		FloatLiteralNode(const Node *consumer, const ITypeName *type, const std::string &value);

	private:
		const Node *m_consumer;
		const ITypeName *m_type;

		std::string m_value;
};

#endif // FLOATLITERALNODE_H
