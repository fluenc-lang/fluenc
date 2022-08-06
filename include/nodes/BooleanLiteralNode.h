#ifndef BOOLEANLITERALNODE_H
#define BOOLEANLITERALNODE_H

#include "Node.h"

class ScalarValue;

class BooleanLiteralNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		BooleanLiteralNode(const Node *consumer, const std::string &value);

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const override;

	private:
		const ScalarValue *resolveValue(const EntryPoint &entryPoint) const;

		const Node *m_consumer;

		std::string m_value;
};

#endif // BOOLEANLITERALNODE_H
