#include <llvm/IR/Constants.h>

#include "DzBooleanLiteral.h"

DzBooleanLiteral::DzBooleanLiteral(DzValue *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}

llvm::Value *DzBooleanLiteral::resolveValue(const EntryPoint &entryPoint) const
{
	auto &context = entryPoint.context();

	auto type = llvm::Type::getInt1Ty(*context);

	if (m_value == "true")
	{
		return llvm::ConstantInt::get(type, 1);
	}

	if (m_value == "false")
	{
		return llvm::ConstantInt::get(type, 0);
	}

	throw new std::exception(); // TODO
}

std::vector<DzResult> DzBooleanLiteral::build(const EntryPoint &entryPoint, Stack values) const
{
	auto value = resolveValue(entryPoint);

	values.push(value);

	return m_consumer->build(entryPoint, values);
}
