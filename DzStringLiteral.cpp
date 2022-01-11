#include <llvm/IR/IRBuilder.h>

#include "DzStringLiteral.h"
#include "IRBuilderEx.h"

#include "types/StringType.h"
#include "values/TypedValue.h"

DzStringLiteral::DzStringLiteral(DzValue *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}

std::vector<DzResult> DzStringLiteral::build(const EntryPoint &entryPoint, Stack values) const
{
	IRBuilderEx builder(entryPoint);

	// I hate ANTLR... Wish I could do this in the grammar file instead.
	auto string = m_value.substr(1, m_value.size() - 2);

	auto address = builder.createGlobalStringPtr(string, "string");

	values.push(new TypedValue { StringType::instance(), address });

	return m_consumer->build(entryPoint, values);
}
