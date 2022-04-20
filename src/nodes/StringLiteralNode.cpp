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

	// I hate ANTLR... Wish I could do this in the grammar file instead.
	auto string = m_value.substr(1, m_value.size() - 2);

	auto address = new ReferenceValue(StringType::instance()
		, builder.createGlobalStringPtr(string, "string")
		);

	values.push(new StringValue(address, id(), string.size()));

	return m_consumer->build(entryPoint, values);
}
