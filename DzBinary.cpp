#include <llvm/IR/IRBuilder.h>

#include "DzBinary.h"
#include "EntryPoint.h"

DzBinary::DzBinary(DzValue *consumer, const std::string &op)
	: m_consumer(consumer)
	, m_op(op)
{
}

std::vector<DzResult> DzBinary::build(const EntryPoint &entryPoint, Stack values) const
{
	auto left = values.pop();
	auto right = values.pop();

	auto value = resolveOp(entryPoint, left, right);

	values.push(value);

	return m_consumer->build(entryPoint, values);
}

TypedValue DzBinary::resolveOp(const EntryPoint &entryPoint, const TypedValue &left, const TypedValue &right) const
{
	auto block = entryPoint.block();

	llvm::IRBuilder<> builder(block);

	if (m_op == "+")
	{
		return TypedValue(left.type(), builder.CreateAdd(left, right));
	}

	if (m_op == "-")
	{
		return TypedValue(left.type(), builder.CreateSub(left, right));
	}

	if (m_op == "*")
	{
		return TypedValue(left.type(), builder.CreateMul(left, right));
	}

	if (m_op == "/")
	{
		return TypedValue(left.type(), builder.CreateSDiv(left, right));
	}

	if (m_op == "<")
	{
		return TypedValue(BooleanType::instance(), builder.CreateCmp(llvm::CmpInst::Predicate::ICMP_SLT, left, right));
	}

	if (m_op == "<=")
	{
		return TypedValue(BooleanType::instance(), builder.CreateCmp(llvm::CmpInst::Predicate::ICMP_SLE, left, right));
	}

	if (m_op == ">")
	{
		return TypedValue(BooleanType::instance(), builder.CreateCmp(llvm::CmpInst::Predicate::ICMP_SGT, left, right));
	}

	if (m_op == ">=")
	{
		return TypedValue(BooleanType::instance(), builder.CreateCmp(llvm::CmpInst::Predicate::ICMP_SGE, left, right));
	}

	if (m_op == "==")
	{
		return TypedValue(BooleanType::instance(), builder.CreateCmp(llvm::CmpInst::Predicate::ICMP_EQ, left, right));
	}

	if (m_op == "!=")
	{
		return TypedValue(BooleanType::instance(), builder.CreateCmp(llvm::CmpInst::Predicate::ICMP_NE, left, right));
	}

	throw new std::exception();
}
