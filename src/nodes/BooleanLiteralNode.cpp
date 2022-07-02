#include <llvm/IR/Constants.h>

#include "nodes/BooleanLiteralNode.h"

#include "types/BooleanType.h"
#include "values/ScalarValue.h"

BooleanLiteralNode::BooleanLiteralNode(const Node *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}

const ScalarValue *BooleanLiteralNode::resolveValue(const EntryPoint &entryPoint) const
{
	auto context = entryPoint.context();

	auto type = BooleanType::instance();

	if (m_value == "true")
	{
		return new ScalarValue { type, llvm::ConstantInt::get(type->storageType(*context), 1) };
	}

	if (m_value == "false")
	{
		return new ScalarValue { type, llvm::ConstantInt::get(type->storageType(*context), 0) };
	}

	throw new std::exception(); // TODO
}

std::vector<DzResult> BooleanLiteralNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto value = resolveValue(entryPoint);

	values.push(value);

	return m_consumer->build(entryPoint, values);
}
