#include <llvm/IR/IRBuilder.h>

#include "DzBinaryNg.h"
#include "EntryPointInfo.h"

DzBinaryNg::DzBinaryNg(const std::string &op
	, DzValueNg *left
	, DzValueNg *right
	)
	: m_op(op)
	, m_left(left)
	, m_right(right)
{
}

llvm::Value *DzBinaryNg::build(const EntryPointInfo &entryPoint) const
{
	auto left = m_left->build(entryPoint);
	auto right = m_right->build(entryPoint);

	llvm::IRBuilder<> builder(entryPoint.block());

	return builder.CreateBinOp(llvm::Instruction::Add, left, right);
}
