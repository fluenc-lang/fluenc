#ifndef EXPANSION_H
#define EXPANSION_H

#include "Node.h"

class Expansion : public Node
{
	public:
		Expansion(Node *consumer);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		Node *m_consumer;
};

#endif // EXPANSION_H
