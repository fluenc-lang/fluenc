#ifndef DZBOOLEANLITERAL_H
#define DZBOOLEANLITERAL_H

#include "Node.h"

class TypedValue;

class BooleanLiteral : public Node
{
	public:
		BooleanLiteral(Node *consumer, const std::string &value);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const TypedValue *resolveValue(const EntryPoint &entryPoint) const;

		Node *m_consumer;

		std::string m_value;
};

#endif // DZBOOLEANLITERAL_H
