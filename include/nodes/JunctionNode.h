#ifndef JUNCTIONNODE_H
#define JUNCTIONNODE_H

#include "Node.h"

class JunctionNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		JunctionNode(const Node *subject);

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Node *m_subject;
};

#endif // JUNCTIONNODE_H
