#include "types/IntegerOperatorSet.h"

#include "IRBuilderEx.h"

const ScalarValue *IntegerOperatorSet::resolve(const std::string &op, const IRBuilderEx &builder, const ScalarValue *left, const ScalarValue *right) const
{
	if (op == "+")
	{
		return builder.createAdd(left, right);
	}

	if (op == "-")
	{
		return builder.createSub(left, right);
	}

	if (op == "*")
	{
		return builder.createMul(left, right);
	}

	if (op == "/")
	{
		return builder.createSDiv(left, right);
	}

	if (op == "<")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SLT, left, right);
	}

	if (op == "<=")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SLE, left, right);
	}

	if (op == ">")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SGT, left, right);
	}

	if (op == ">=")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SGE, left, right);
	}

	if (op == "==")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::ICMP_EQ, left, right);
	}

	if (op == "!=")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::ICMP_NE, left, right);
	}

	if (op == "%")
	{
		return builder.createSRem(left, right);
	}

	if (op == "|")
	{
		return builder.createOr(left, right);
	}

	if (op == "&")
	{
		return builder.createAnd(left, right);
	}

	if (op == "^")
	{
		return builder.createXor(left, right);
	}

	throw new std::exception();
}
