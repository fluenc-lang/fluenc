#ifndef JUNCTIONNODE_H
#define JUNCTIONNODE_H

#include "Node.h"

class JunctionNode : public VisitableNode<JunctionNode>
{
	friend class Emitter;

	public:
		JunctionNode(const Node *subject);

	private:
		const Node *m_subject;
};

#endif // JUNCTIONNODE_H
