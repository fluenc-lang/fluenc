#ifndef ARRAYSINKNODE_H
#define ARRAYSINKNODE_H

#include "Node.h"

class Type;

class ArraySinkNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		ArraySinkNode(size_t size
			, const Node *consumer
			, const Node *firstValue
			);

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, DefaultVisitorContext context) const override;

	private:
		size_t m_size;

		const Node *m_consumer;
		const Node *m_firstValue;
};

#endif // ARRAYSINKNODE_H
