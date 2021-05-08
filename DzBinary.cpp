#include <llvm/IR/IRBuilder.h>

#include "DzBinary.h"
#include "VisitorV2.h"
#include "EntryPoint.h"

DzBinary::DzBinary()
	: m_left(nullptr)
	, m_right(nullptr)
{
}

DzValue *DzBinary::left() const
{
	return m_left;
}

void DzBinary::setLeft(DzValue *left)
{
	m_left = left;
}

DzValue *DzBinary::right() const
{
	return m_right;
}

void DzBinary::setRight(DzValue *right)
{
	m_right = right;
}

llvm::Value *DzBinary::build(const EntryPoint &entryPoint) const
{
	auto left = m_left->build(entryPoint);
	auto right = m_right->build(entryPoint);

	llvm::IRBuilder<> builder(entryPoint.block());

	return builder.CreateBinOp(llvm::Instruction::Add, left, right);
}
