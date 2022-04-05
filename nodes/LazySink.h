#ifndef LAZYSINK_H
#define LAZYSINK_H

#include "Node.h"

class Type;

class LazySink : public Node
{
	public:
		LazySink(const Type *iteratorType
			, const Node *consumer
			, const Node *subject
			);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Type *m_iteratorType;

		const Node *m_consumer;
		const Node *m_subject;
};

#endif // LAZYSINK_H
