#include <sstream>

#include <llvm/IR/Type.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/IRBuilder.h>

#include "DzReturnNg.h"
#include "EntryPointInfo.h"

DzReturnNg::DzReturnNg(DzValueNg *value, DzValueNg *consumer)
	: m_value(value)
{
}

llvm::Value *DzReturnNg::build(const EntryPointInfo &entryPoint) const
{
	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	std::ostringstream ss;
	ss << entryPoint.name();
	ss << "_ret";

	auto k = ss.str();

	auto c = module->getOrInsertGlobal(k, llvm::Type::getInt32Ty(*context), [&]
	{
		auto initializer = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 0);

		return new llvm::GlobalVariable(*module, llvm::Type::getInt32Ty(*context), false, llvm::GlobalValue::InternalLinkage, initializer, k);
	});

	auto v = m_value->build(entryPoint);

	llvm::IRBuilder<> builder(entryPoint.block());
	builder.CreateStore(v, c);

	return c;
}
