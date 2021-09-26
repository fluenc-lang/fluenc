#include <llvm/IR/Constants.h>

#include "DzIntegralLiteral.h"
#include "DzTypeName.h"
#include "EntryPoint.h"
#include "Type.h"

DzIntegralLiteral::DzIntegralLiteral(DzValue *consumer, DzTypeName *type, const std::string &value)
	: m_consumer(consumer)
	, m_type(type)
	, m_value(value)
{
}

std::vector<DzResult> DzIntegralLiteral::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();

	auto type = m_type->resolve(entryPoint);
	auto storageType = type->storageType(*context);

	auto value = TypedValue(type
		,  llvm::ConstantInt::get((llvm::IntegerType *)storageType, m_value, 10)
		);

	values.push(value);

	return m_consumer->build(entryPoint, values);
}
