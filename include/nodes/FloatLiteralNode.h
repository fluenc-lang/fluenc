#ifndef FLOATLITERALNODE_H
#define FLOATLITERALNODE_H

#include "Node.h"

class ITypeName;

class FloatLiteralNode : public Node
{
	public:
		FloatLiteralNode(const Node *consumer, const ITypeName *type, const std::string &value);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Node *m_consumer;
		const ITypeName *m_type;

		std::string m_value;
};

#endif // FLOATLITERALNODE_H
