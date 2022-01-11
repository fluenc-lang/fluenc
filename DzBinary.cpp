#include <llvm/IR/IRBuilder.h>

#include "DzBinary.h"
#include "EntryPoint.h"
#include "IRBuilderEx.h"

#include "types/BooleanType.h"
#include "values/TypedValue.h"

DzBinary::DzBinary(DzValue *consumer, const std::string &op)
	: m_consumer(consumer)
	, m_op(op)
{
}

std::vector<DzResult> DzBinary::build(const EntryPoint &entryPoint, Stack values) const
{
	auto left = values.require<TypedValue>();
	auto right = values.require<TypedValue>();

	auto value = resolveOp(entryPoint, left, right);

	values.push(value);

	return m_consumer->build(entryPoint, values);
}

const TypedValue *DzBinary::resolveOp(const EntryPoint &entryPoint, const TypedValue *left, const TypedValue *right) const
{
	IRBuilderEx builder(entryPoint);

	if (m_op == "+")
	{
		return new TypedValue { left->type(), builder.createAdd(*left, *right) };
	}

	if (m_op == "-")
	{
		return new TypedValue { left->type(), builder.createSub(*left, *right) };
	}

	if (m_op == "*")
	{
		return new TypedValue { left->type(), builder.createMul(*left, *right) };
	}

	if (m_op == "/")
	{
		return new TypedValue { left->type(), builder.createSDiv(*left, *right) };
	}

	if (m_op == "<")
	{
		return new TypedValue { BooleanType::instance(), builder.createCmp(llvm::CmpInst::Predicate::ICMP_SLT, *left, *right) };
	}

	if (m_op == "<=")
	{
		return new TypedValue { BooleanType::instance(), builder.createCmp(llvm::CmpInst::Predicate::ICMP_SLE, *left, *right) };
	}

	if (m_op == ">")
	{
		return new TypedValue { BooleanType::instance(), builder.createCmp(llvm::CmpInst::Predicate::ICMP_SGT, *left, *right) };
	}

	if (m_op == ">=")
	{
		return new TypedValue { BooleanType::instance(), builder.createCmp(llvm::CmpInst::Predicate::ICMP_SGE, *left, *right) };
	}

	if (m_op == "==")
	{
		return new TypedValue { BooleanType::instance(), builder.createCmp(llvm::CmpInst::Predicate::ICMP_EQ, *left, *right) };
	}

	if (m_op == "!=")
	{
		return new TypedValue { BooleanType::instance(), builder.createCmp(llvm::CmpInst::Predicate::ICMP_NE, *left, *right) };
	}

	if (m_op == "&&")
	{
		return new TypedValue { BooleanType::instance(), builder.createAnd(*left, *right) };
	}

	if (m_op == "||")
	{
		return new TypedValue { BooleanType::instance(), builder.createOr(*left, *right) };
	}

	if (m_op == "%")
	{
		return new TypedValue { left->type(), builder.createSRem(*left, *right) };
	}

	throw new std::exception();
}
