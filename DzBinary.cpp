#include <llvm/IR/IRBuilder.h>

#include "DzBinary.h"
#include "EntryPoint.h"

DzBinary::DzBinary(DzValue *consumer, const std::string &op)
	: m_consumer(consumer)
	, m_op(op)
{
}

llvm::Value *DzBinary::build(const EntryPoint &entryPoint, std::deque<llvm::Value *> &values) const
{
	auto op = resolveOp(entryPoint, values);

	values.push_back(op);

	return m_consumer->build(entryPoint, values);
}

llvm::Value *DzBinary::resolveOp(const EntryPoint &entryPoint, std::deque<llvm::Value *> &values) const
{
	auto block = entryPoint.block();

	auto left = values.back();

	values.pop_back();

	auto right = values.back();

	values.pop_back();

	llvm::IRBuilder<> builder(block);

	if (m_op == "+")
	{
		return builder.CreateAdd(left, right);
	}

	if (m_op == "-")
	{
		return builder.CreateSub(left, right);
	}

	if (m_op == "*")
	{
		return builder.CreateMul(left, right);
	}

	if (m_op == "/")
	{
		return builder.CreateSDiv(left, right);
	}

	if (m_op == "<")
	{
		return builder.CreateCmp(llvm::CmpInst::Predicate::ICMP_SLT, left, right);
	}

	if (m_op == "<=")
	{
		return builder.CreateCmp(llvm::CmpInst::Predicate::ICMP_SLE, left, right);
	}

	if (m_op == ">")
	{
		return builder.CreateCmp(llvm::CmpInst::Predicate::ICMP_SGT, left, right);
	}

	if (m_op == ">=")
	{
		return builder.CreateCmp(llvm::CmpInst::Predicate::ICMP_SGE, left, right);
	}

	if (m_op == "==")
	{
		return builder.CreateCmp(llvm::CmpInst::Predicate::ICMP_EQ, left, right);
	}

	if (m_op == "!=")
	{
		return builder.CreateCmp(llvm::CmpInst::Predicate::ICMP_NE, left, right);
	}

	return nullptr;
}
