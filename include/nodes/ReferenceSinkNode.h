#ifndef REFERENCESINKNODE_H
#define REFERENCESINKNODE_H

#include "Node.h"

class ReferenceSinkNode : public VisitableNode<ReferenceSinkNode>
{
	friend class Emitter;
	friend class Analyzer;

	public:
		ReferenceSinkNode(const Node *consumer);

		const BaseValue *makeReference(const BaseValue *value, const EntryPoint &entryPoint) const;

	private:
		const Node *m_consumer;
};

#endif // REFERENCESINKNODE_H
