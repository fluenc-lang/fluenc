#ifndef STRINGLITERALNODE_H
#define STRINGLITERALNODE_H

#include "Node.h"

class StringLiteralNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		StringLiteralNode(const Node *consumer, const std::string &value);

		std::vector<DzResult<BaseValue>> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;

	private:
		const Node *m_consumer;

		std::string m_value;
};

#endif // STRINGLITERALNODE_H
