#ifndef CONTINUATION_H
#define CONTINUATION_H

#include "Node.h"

class Continuation : public Node
{
	public:
		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;
};

#endif // CONTINUATION_H
