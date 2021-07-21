#include <llvm/IR/IRBuilder.h>

#include <iostream>

#include "DzReturn.h"
#include "EntryPoint.h"

DzReturn::DzReturn(DzValue *value)
	: m_value(value)
{
}

DzValue *DzReturn::value() const
{
	return m_value;
}

llvm::Value *DzReturn::build(const EntryPoint &entryPoint) const
{
	llvm::IRBuilder<> builder(entryPoint.block());

	auto value = m_value->build(entryPoint);
	auto ret = builder.CreateRet(value);

	return ret;
}
