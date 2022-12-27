#ifndef POD_H
#define POD_H

#include "Node.h"

class Pod : public Node
{
	public:
		Pod(const Node *subject, const Stack &outputValues);

		std::vector<DzResult> accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const;

	private:
		const Node *m_subject;

		const Stack m_outputValues;
};

#endif // POD_H
