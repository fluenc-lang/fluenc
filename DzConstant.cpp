#include <llvm/IR/Constants.h>

#include "DzConstant.h"
#include "EntryPoint.h"

DzConstant::DzConstant(DzValue *consumer, std::string value)
	: m_consumer(consumer)
	, m_value(value)
{
}

llvm::Value *DzConstant::build(const EntryPoint &entryPoint, std::deque<llvm::Value *> &values) const
{
	auto &context = entryPoint.context();

	auto value = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), m_value, 10);

	values.push_back(value);

	return m_consumer->build(entryPoint, values);
}
