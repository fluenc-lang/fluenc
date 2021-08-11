#include <llvm/IR/Constants.h>

#include "DzConstant.h"
#include "EntryPoint.h"

DzConstant::DzConstant(DzValue *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}

std::vector<DzResult> DzConstant::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();

	auto value = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), m_value, 10);

	values.push(value);

	return m_consumer->build(entryPoint, values);
}
