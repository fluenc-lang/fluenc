#include "types/BooleanOperatorSet.h"

#include "IRBuilderEx.h"

const ScalarValue *BooleanOperatorSet::resolve(const std::string &op, const IRBuilderEx &builder, const ScalarValue *left, const ScalarValue *right) const
{
	if (op == "==")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::ICMP_EQ, left, right);
	}

	if (op == "!=")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::ICMP_NE, left, right);
	}

	if (op == "&&")
	{
		return builder.createLogicalAnd(left, right);
	}

	if (op == "||")
	{
		return builder.createLogicalOr(left, right);
	}

	throw new std::exception();
}
