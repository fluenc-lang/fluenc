#include <llvm/IR/Constants.h>

#include "nodes/FloatLiteralNode.h"

#include "values/ScalarValue.h"

#include "ITypeName.h"

FloatLiteralNode::FloatLiteralNode(const Node *consumer, const ITypeName *type, const std::string &value)
	: m_consumer(consumer)
	, m_type(type)
	, m_value(value)
{
}

std::vector<DzResult> FloatLiteralNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto context = entryPoint.context();

	auto type = m_type->resolve(entryPoint);
	auto storageType = type->storageType(*context);

	auto value = new ScalarValue(type
		, llvm::ConstantFP::get(storageType, m_value)
		);

	values.push(value);

	return m_consumer->build(entryPoint, values);
}
