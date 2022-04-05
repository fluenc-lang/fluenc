#ifndef ARRAYCONTINUATIONNODE_H
#define ARRAYCONTINUATIONNODE_H

#include "Node.h"

class ArrayContinuationNode : public Node
{
	public:
		ArrayContinuationNode(llvm::Value *index);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		llvm::Value *m_index;
};

#endif // ARRAYCONTINUATIONNODE_H
