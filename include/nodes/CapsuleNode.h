#ifndef CAPSULENODE_H
#define CAPSULENODE_H

#include "Node.h"

class CapsuleNode : public Node
{
	public:
		CapsuleNode(const Stack &values, const Node *subject);

	private:
		Stack m_values;

		const Node *m_call;
};

#endif // CAPSULENODE_H
