#ifndef DZBINARY_H
#define DZBINARY_H

#include "Node.h"

class TypedValue;

class Binary : public Node
{
	public:
		Binary(Node *consumer, const std::string &op);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const TypedValue *resolveOp(const EntryPoint &entryPoint, const TypedValue *left, const TypedValue *right) const;

		Node * m_consumer;

		std::string m_op;
};

#endif // DZBINARY_H
