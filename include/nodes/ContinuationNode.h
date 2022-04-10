#ifndef CONTINUATIONNODE_H
#define CONTINUATIONNODE_H

#include "Node.h"

class ContinuationNode : public Node
{
	public:
		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;
};

#endif // CONTINUATIONNODE_H
