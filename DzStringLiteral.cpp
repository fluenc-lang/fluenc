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

	auto address = builder.CreateGlobalStringPtr(m_value, "", 0, module.get());

	values.push(address);

	return m_consumer->build(entryPoint, values);
}
