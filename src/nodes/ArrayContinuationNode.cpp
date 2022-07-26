#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

#include "IRBuilderEx.h"

#include "nodes/ArrayContinuationNode.h"
#include "nodes/ContinuationNode.h"

#include "values/ScalarValue.h"
#include "values/ExpandedValue.h"

#include "types/Int64Type.h"

ArrayContinuationNode::ArrayContinuationNode(const ReferenceValue *index, const Node *node, const Type *iteratorType)
	: m_index(index)
	, m_node(node)
	, m_iteratorType(iteratorType)
{
}

std::vector<DzResult> ArrayContinuationNode::build(const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	auto context = entryPoint.context();

	auto indexType = Int64Type::instance();
	auto storageType = indexType->storageType(*context);

	auto indexConstant = new ScalarValue(indexType
		, llvm::ConstantInt::get(storageType, 1)
		);

	IRBuilderEx builder(entryPoint);

	auto load = builder.createLoad(m_index, "index");
	auto add = builder.createAdd(load, indexConstant);

	builder.createStore(add, m_index);

	auto value = new ExpandedValue(true
		, m_iteratorType
		, entryPoint
		, m_node
		, this
		, std::vector<const ExpandedValue *>()
		);

	return {{ entryPoint, value }};
}
