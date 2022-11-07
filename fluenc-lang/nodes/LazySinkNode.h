#ifndef LAZYSINKNODE_H
#define LAZYSINKNODE_H

#include "Node.h"

class Type;

class LazySinkNode : public Node
{
	public:
		LazySinkNode(const Type *iteratorType
			, const Node *consumer
			, const Node *subject
			);

	private:
		const Type *m_iteratorType;

		const Node *m_consumer;
		const Node *m_subject;
};

#endif // LAZYSINKNODE_H
