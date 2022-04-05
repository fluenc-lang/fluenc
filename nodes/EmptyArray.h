#ifndef DZEMPTYARRAY_H
#define DZEMPTYARRAY_H

#include "Node.h"

class EmptyArray : public Node
{
	public:
		EmptyArray(Node *consumer);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		Node *m_consumer;
};

#endif // DZEMPTYARRAY_H
