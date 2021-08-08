#include <llvm/IR/IRBuilder.h>

#include "DzFunctionNg.h"
#include "EntryPointInfo.h"

DzFunctionNg::DzFunctionNg(DzValueNg *block
	, const std::string &name
	, const std::vector<DzMemberNg *> arguments
	)
	: m_name(name)
	, m_arguments(arguments)
	, m_block(block)
{
}

std::string DzFunctionNg::name() const
{
	return m_name;
}

std::vector<DzMemberNg *> DzFunctionNg::arguments() const
{
	return m_arguments;
}

FunctionAttribute DzFunctionNg::attribute() const
{
	return FunctionAttribute::None;
}

DzTypeNameNg *DzFunctionNg::returnType() const
{
	return nullptr;
}

llvm::Value *DzFunctionNg::build(const EntryPointInfo &entryPoint) const
{
	auto block = entryPoint.block();

	llvm::IRBuilder<> builder(block);

	auto ep = entryPoint
		.withName(block->getName().str());

	return m_block->build(ep);
}
