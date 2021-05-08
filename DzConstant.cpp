#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>

#include "DzConstant.h"
#include "EntryPoint.h"

DzConstant::DzConstant(std::string value)
	: m_value(value)
{
}

llvm::Value *DzConstant::build(const EntryPoint &context) const
{
	auto type = llvm::Type::getInt32Ty(*context.context());

	return llvm::ConstantInt::get(type, m_value, 10);
}
