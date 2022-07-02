#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

#include "nodes/ArrayElementNode.h"
#include "IRBuilderEx.h"

#include "nodes/ArrayContinuationNode.h"

#include "types/IteratorType.h"

#include "values/ReferenceValue.h"
#include "values/ExpandableValue.h"
#include "values/TupleValue.h"
#include "values/IndexedValue.h"
#include "values/ScalarValue.h"

ArrayElementNode::ArrayElementNode(const Type *arrayType, const Node *node, const Node *next)
	: m_arrayType(arrayType)
	, m_node(node)
	, m_next(next)
{
}

std::vector<DzResult> ArrayElementNode::build(const EntryPoint &entryPoint, Stack values) const
{
	Stack valuesIfTrue;

	auto context = entryPoint.context();
	auto module = entryPoint.module();

	auto block = entryPoint.block();
	auto function = entryPoint.function();

	auto dataLayout = module->getDataLayout();

	auto index = values.require<ReferenceValue>(nullptr);
	auto value = values.require<IndexedValue>(nullptr);

	if (m_next)
	{
		auto valuesIfFalse = values;

		insertBlock(block, function);

		auto indexType = index->type();
		auto storageType = indexType->storageType(*context);

		auto ifTrue = llvm::BasicBlock::Create(*context);
		auto ifFalse = llvm::BasicBlock::Create(*context);

		IRBuilderEx builder(entryPoint);

		auto indexLoad = builder.createLoad(index, "index");

		auto indexConstant = new ScalarValue(indexType
			, llvm::ConstantInt::get(storageType, value->index())
			);

		auto comparison =  builder.createCmp(llvm::CmpInst::Predicate::ICMP_EQ, indexLoad, indexConstant);

		builder.createCondBr(comparison , ifTrue, ifFalse);

		auto epIfFalse = entryPoint
			.withIndex(value->index())
			.withBlock(ifFalse);

		auto epIfTrue = entryPoint
			.withIndex(value->index())
			.withBlock(ifTrue);

		valuesIfFalse.push(index);

		auto continuation = new ArrayContinuationNode(index, m_node, IteratorType::instance());
		auto expandable = new ExpandableValue(m_arrayType, entryPoint, continuation);

		auto tuple = new TupleValue({ expandable, value->subject() });

		valuesIfTrue.push(tuple);

		auto resultsIfFalse = m_next->build(epIfFalse, valuesIfFalse);

		std::vector<DzResult> result = {{ epIfTrue, valuesIfTrue }};

		result.insert(end(result), begin(resultsIfFalse), end(resultsIfFalse));

		return result;
	}

	auto ep = entryPoint
		.withIndex(value->index());

	values.push(value->subject());

	return {{ ep, values }};
}
