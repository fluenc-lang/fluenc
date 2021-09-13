#include <llvm/IR/Constants.h>

#include "DzBooleanLiteral.h"

DzBooleanLiteral::DzBooleanLiteral(DzValue *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}

TypedValue DzBooleanLiteral::resolveValue(const EntryPoint &entryPoint) const
{
	auto &context = entryPoint.context();

	auto type = BooleanType::instance();

	if (m_value == "true")
	{
		return { type, llvm::ConstantInt::get(type->storageType(*context), 1) };
	}

	if (m_value == "false")
	{
		return { type, llvm::ConstantInt::get(type->storageType(*context), 0) };
	}

	throw new std::exception(); // TODO
}

std::vector<DzResult> DzBooleanLiteral::build(const EntryPoint &entryPoint, Stack values) const
{
	auto value = resolveValue(entryPoint);

	values.push(value);

	return m_consumer->build(entryPoint, values);
}
