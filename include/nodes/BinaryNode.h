#ifndef BINARYNODE_H
#define BINARYNODE_H

#include "Node.h"

class ScalarValue;

class BinaryNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		BinaryNode(const Node *consumer, const std::string &op);

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Node * m_consumer;

		std::string m_op;
};

#endif // BINARYNODE_H
