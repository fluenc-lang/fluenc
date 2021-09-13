#include <llvm/IR/IRBuilder.h>

#include "DzStringLiteral.h"

DzStringLiteral::DzStringLiteral(DzValue *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}

std::vector<DzResult> DzStringLiteral::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &module = entryPoint.module();
	auto block = entryPoint.block();

	llvm::IRBuilder<> builder(block);

	// I hate ANTLR... Wish I could do this in the grammar file instead.
	auto string = m_value.substr(1, m_value.size() - 2);

	auto address = builder.CreateGlobalStringPtr(string, "string", 0, module.get());

	values.push(TypedValue(StringType::instance(), address));

	return m_consumer->build(entryPoint, values);
}
