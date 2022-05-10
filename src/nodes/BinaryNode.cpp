#include <llvm/IR/IRBuilder.h>

#include "EntryPoint.h"
#include "IRBuilderEx.h"
#include "ValueHelper.h"

#include "nodes/BinaryNode.h"

#include "types/BooleanType.h"

#include "values/ScalarValue.h"
#include "values/ReferenceValue.h"

BinaryNode::BinaryNode(const Node *consumer, const std::string &op)
	: m_consumer(consumer)
	, m_op(op)
{
}

std::vector<DzResult> BinaryNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto left = ValueHelper::getScalar(entryPoint, values);
	auto right = ValueHelper::getScalar(entryPoint, values);

	auto value = resolveOp(entryPoint, left, right);

	values.push(value);

	return m_consumer->build(entryPoint, values);
}

const ScalarValue *BinaryNode::resolveOp(const EntryPoint &entryPoint, const ScalarValue *left, const ScalarValue *right) const
{
	IRBuilderEx builder(entryPoint);

	if (m_op == "+")
	{
		return builder.createAdd(left, right);
	}

	if (m_op == "-")
	{
		return builder.createSub(left, right);
	}

	if (m_op == "*")
	{
		return builder.createMul(left, right);
	}

	if (m_op == "/")
	{
		return builder.createSDiv(left, right);
	}

	if (m_op == "<")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SLT, left, right);
	}

	if (m_op == "<=")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SLE, left, right);
	}

	if (m_op == ">")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SGT, left, right);
	}

	if (m_op == ">=")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SGE, left, right);
	}

	if (m_op == "==")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::ICMP_EQ, left, right);
	}

	if (m_op == "!=")
	{
		return builder.createCmp(llvm::CmpInst::Predicate::ICMP_NE, left, right);
	}

	if (m_op == "&&")
	{
		return builder.createAnd(left, right);
	}

	if (m_op == "||")
	{
		return builder.createOr(left, right);
	}

	if (m_op == "%")
	{
		return builder.createSRem(left, right);
	}

	throw new std::exception();
}
