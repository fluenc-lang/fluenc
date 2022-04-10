#ifndef TAINTEDSINKNODE_H
#define TAINTEDSINKNODE_H

#include "Node.h"

class TaintedSinkNode : public Node
{
	public:
		TaintedSinkNode(Node *subject);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		Node *m_subject;
};

#endif // TAINTEDSINKNODE_H
