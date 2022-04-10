#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

#include "nodes/ArrayContinuationNode.h"
#include "IRBuilderEx.h"

#include "values/ScalarValue.h"

#include "types/Int64Type.h"

ArrayContinuationNode::ArrayContinuationNode(const ReferenceValue *index)
	: m_index(index)
{
}

std::vector<DzResult> ArrayContinuationNode::build(const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	auto &context = entryPoint.context();

	auto indexType = Int64Type::instance();
	auto storageType = indexType->storageType(*context);

	auto indexConstant = new ScalarValue(indexType
		, llvm::ConstantInt::get(storageType, 1)
		);

	IRBuilderEx builder(entryPoint);

	auto load = builder.createLoad(m_index, "index");
	auto add = builder.createAdd(load, indexConstant);

	builder.createStore(add, m_index);

	return {{ entryPoint, Stack() }};
}
