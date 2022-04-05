#ifndef DZARRAYCONTINUATION_H
#define DZARRAYCONTINUATION_H

#include "Node.h"

class ArrayContinuation : public Node
{
	public:
		ArrayContinuation(llvm::Value *index);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		llvm::Value *m_index;
};

#endif // DZARRAYCONTINUATION_H
