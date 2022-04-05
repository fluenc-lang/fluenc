#include <llvm/IR/Constants.h>

#include "BooleanLiteral.h"

#include "types/BooleanType.h"
#include "values/TypedValue.h"

BooleanLiteral::BooleanLiteral(Node *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}

const TypedValue *BooleanLiteral::resolveValue(const EntryPoint &entryPoint) const
{
	auto &context = entryPoint.context();

	auto type = BooleanType::instance();

	if (m_value == "true")
	{
		return new TypedValue { type, llvm::ConstantInt::get(type->storageType(*context), 1) };
	}

	if (m_value == "false")
	{
		return new TypedValue { type, llvm::ConstantInt::get(type->storageType(*context), 0) };
	}

	throw new std::exception(); // TODO
}

std::vector<DzResult> BooleanLiteral::build(const EntryPoint &entryPoint, Stack values) const
{
	auto value = resolveValue(entryPoint);

	values.push(value);

	return m_consumer->build(entryPoint, values);
}
