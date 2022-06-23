#include "types/FloatOperatorSet.h"

#include "IRBuilderEx.h"

const ScalarValue *FloatOperatorSet::resolve(const std::string &op, const IRBuilderEx &builder, const ScalarValue *left, const ScalarValue *right) const
{
	if (op == "+")
	{
		return builder.createFAdd(left, right);
	}

	if (op == "-")
	{
		return builder.createFSub(left, right);
	}

	if (op == "*")
	{
		return builder.createFMul(left, right);
	}

	if (op == "/")
	{
		return builder.createFDiv(left, right);
	}

	if (op == "<")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::FCMP_ULT, left, right);
	}

	if (op == "<=")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::FCMP_ULE, left, right);
	}

	if (op == ">")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::FCMP_UGT, left, right);
	}

	if (op == ">=")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::FCMP_UGE, left, right);
	}

	if (op == "==")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::FCMP_UEQ, left, right);
	}

	if (op == "!=")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::FCMP_UNE, left, right);
	}

	throw new std::exception();
}
