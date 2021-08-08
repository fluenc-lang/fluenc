#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Constants.h>

#include "DzConstantNg.h"
#include "EntryPointInfo.h"

DzConstantNg::DzConstantNg(const std::string &value)
	: m_value(value)
{
}

llvm::Value *DzConstantNg::build(const EntryPointInfo &entryPoint) const
{
	auto type = llvm::Type::getInt32Ty(*entryPoint.context());

	return llvm::ConstantInt::get(type, m_value, 10);
}
