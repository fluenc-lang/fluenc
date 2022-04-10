#ifndef REFERENCESINKNODE_H
#define REFERENCESINKNODE_H

#include "Node.h"

class ReferenceSinkNode : public Node
{
	public:
		ReferenceSinkNode(const Node *consumer);

		const BaseValue *makeReference(const BaseValue *value, const EntryPoint &entryPoint) const;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Node *m_consumer;
};

#endif // REFERENCESINKNODE_H
