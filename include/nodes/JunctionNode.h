#ifndef JUNCTIONNODE_H
#define JUNCTIONNODE_H

#include "Node.h"

class JunctionNode : public Node
{
	friend class Emitter;

	struct SingleResult
	{
		const EntryPoint entryPoint;
		const BaseValue *value;
	};

	public:
		JunctionNode(const Node *subject);

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Node *m_subject;
};

#endif // JUNCTIONNODE_H
