#ifndef DZCONDITIONAL_H
#define DZCONDITIONAL_H

#include "Node.h"

class Conditional : public Node
{
	public:
		Conditional(Node *ifFalse, Node *ifTrue);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		Node *m_ifTrue;
		Node *m_ifFalse;
};

#endif // DZCONDITIONAL_H
