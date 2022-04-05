#ifndef REFERENCESINK_H
#define REFERENCESINK_H

#include "Node.h"

class ReferenceSink : public Node
{
	public:
		ReferenceSink(const Node *consumer);

		const BaseValue *makeReference(const BaseValue *value, const EntryPoint &entryPoint) const;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Node *m_consumer;
};

#endif // REFERENCESINK_H
