#include "IRBuilderEx.h"

#include "nodes/StringLiteralNode.h"

#include "values/StringValue.h"
#include "values/ReferenceValue.h"

#include "types/StringType.h"

StringLiteralNode::StringLiteralNode(const Node *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}

std::vector<DzResult> StringLiteralNode::build(const EntryPoint &entryPoint, Stack values) const
{
	IRBuilderEx builder(entryPoint);

	auto address = new ReferenceValue(StringType::instance()
		, builder.createGlobalStringPtr(m_value, "string")
		);

	values.push(new StringValue(address, id(), m_value.size()));

	return m_consumer->build(entryPoint, values);
}
