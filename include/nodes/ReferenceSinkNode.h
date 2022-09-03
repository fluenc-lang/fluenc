#ifndef REFERENCESINKNODE_H
#define REFERENCESINKNODE_H

#include "Node.h"

class ReferenceSinkNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		ReferenceSinkNode(const Node *consumer);

		const BaseValue *makeReference(const BaseValue *value, const EntryPoint &entryPoint) const;

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, DefaultVisitorContext context) const override;

	private:
		const Node *m_consumer;
};

#endif // REFERENCESINKNODE_H
