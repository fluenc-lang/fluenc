#ifndef TAINTEDSINK_H
#define TAINTEDSINK_H

#include "Node.h"

class TaintedSink : public Node
{
	public:
		TaintedSink(Node *subject);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		Node *m_subject;
};

#endif // TAINTEDSINK_H
