#ifndef BINARYNODE_H
#define BINARYNODE_H

#include "Node.h"

class ScalarValue;

class BinaryNode : public Node
{
	public:
		BinaryNode(Node *consumer, const std::string &op);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const ScalarValue *resolveOp(const EntryPoint &entryPoint, const ScalarValue *left, const ScalarValue *right) const;

		Node * m_consumer;

		std::string m_op;
};

#endif // BINARYNODE_H
