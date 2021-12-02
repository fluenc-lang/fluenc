#include <llvm/IR/Constants.h>

#include "DzCharacterLiteral.h"

#include "values/TypedValue.h"

#include "types/Int32Type.h"

DzCharacterLiteral::DzCharacterLiteral(DzValue *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}

std::vector<DzResult> DzCharacterLiteral::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();

	auto type = Int32Type::instance();
	auto storageType = type->storageType(*context);

	auto value = new TypedValue(type
		, llvm::ConstantInt::get((llvm::IntegerType *)storageType
			, *m_value.begin()
			)
		);

	values.push(value);

	return m_consumer->build(entryPoint, values);
}
