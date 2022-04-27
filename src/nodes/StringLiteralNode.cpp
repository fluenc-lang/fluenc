#include "IRBuilderEx.h"

#include "nodes/StringLiteralNode.h"

#include "values/StringValue.h"
#include "values/ReferenceValue.h"
#include "values/ScalarValue.h"

#include "types/StringType.h"

StringLiteralNode::StringLiteralNode(const Node *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}

std::vector<DzResult> StringLiteralNode::build(const EntryPoint &entryPoint, Stack values) const
{
	IRBuilderEx builder(entryPoint);

	auto stringType = StringType::get(m_value.size());

	auto string = new ScalarValue(stringType
		, builder.createGlobalStringPtr(m_value, "string")
		);

	auto alloc = entryPoint.alloc(stringType);

	builder.createStore(string, alloc);

	values.push(new StringValue(alloc, id(), m_value.size()));

	return m_consumer->build(entryPoint, values);
}
