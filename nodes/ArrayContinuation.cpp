#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

#include "ArrayContinuation.h"
#include "IRBuilderEx.h"

#include "types/Int64Type.h"

ArrayContinuation::ArrayContinuation(llvm::Value *index)
	: m_index(index)
{
}

std::vector<DzResult> ArrayContinuation::build(const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	auto &context = entryPoint.context();

	auto indexType = Int64Type::instance();
	auto storageType = indexType->storageType(*context);

	auto indexConstant = llvm::ConstantInt::get(storageType, 1);

	IRBuilderEx builder(entryPoint);

	auto load = builder.createLoad(m_index, "index");
	auto add = builder.createAdd(load, indexConstant);

	builder.createStore(add, m_index);

	return {{ entryPoint, Stack() }};
}
