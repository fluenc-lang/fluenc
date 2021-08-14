#include <llvm/IR/Constants.h>

#include "DzIntegralLiteral.h"
#include "DzTypeName.h"
#include "EntryPoint.h"

DzIntegralLiteral::DzIntegralLiteral(DzValue *consumer, DzTypeName *type, const std::string &value)
	: m_consumer(consumer)
	, m_type(type)
	, m_value(value)
{
}

std::vector<DzResult> DzIntegralLiteral::build(const EntryPoint &entryPoint, Stack values) const
{
	auto type = (llvm::IntegerType *)m_type->resolve(entryPoint);

	auto value = llvm::ConstantInt::get(type, m_value, 10);

	values.push(value);

	return m_consumer->build(entryPoint, values);
}
