#ifndef JUNCTIONNODE_H
#define JUNCTIONNODE_H

#include "Node.h"

class JunctionNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		JunctionNode(const Node *subject);

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, DefaultVisitorContext context) const override;

	private:
		const Node *m_subject;
};

#endif // JUNCTIONNODE_H
