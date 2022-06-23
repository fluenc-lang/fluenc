#include <llvm/IR/IRBuilder.h>

#include "EntryPoint.h"
#include "IRBuilderEx.h"
#include "ValueHelper.h"

#include "nodes/BinaryNode.h"

#include "types/BooleanType.h"
#include "types/IOperatorSet.h"

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

	auto leftType = left->type();
	auto rightType = right->type();

	if (leftType->compatibility(rightType, entryPoint) > 0)
	{
		throw new std::exception(); // TODO
	}

	auto operators = leftType->operators();

	if (!operators)
	{
		throw new std::exception(); // TODO
	}

	IRBuilderEx builder(entryPoint);

	auto value = operators->resolve(m_op, builder, left, right);

	values.push(value);

	return m_consumer->build(entryPoint, values);
}
