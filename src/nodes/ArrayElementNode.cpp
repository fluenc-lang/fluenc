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

ArrayElementNode::ArrayElementNode(const Type *arrayType, const Node *next)
	: m_arrayType(arrayType)
	, m_next(next)
{
}

std::vector<DzResult> ArrayElementNode::build(const EntryPoint &entryPoint, Stack values) const
{
	Stack valuesIfTrue;

	auto &context = entryPoint.context();
	auto &module = entryPoint.module();

	auto block = entryPoint.block();
	auto function = entryPoint.function();

	auto dataLayout = module->getDataLayout();

	auto index = values.require<ReferenceValue>();
	auto value = values.require<IndexedValue>();

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
			.withBlock(ifFalse);

		auto epIfTrue = entryPoint
			.withBlock(ifTrue);

		valuesIfFalse.push(index);

		auto continuation = new ArrayContinuationNode(index);
		auto expandable = new ExpandableValue(m_arrayType, entryPoint, continuation);

		auto tuple = new TupleValue({ expandable, value->subject() });

		valuesIfTrue.push(tuple);

		auto resultsIfFalse = m_next->build(epIfFalse, valuesIfFalse);

		std::vector<DzResult> result = {{ epIfTrue, valuesIfTrue }};

		result.insert(end(result), begin(resultsIfFalse), end(resultsIfFalse));

		return result;
	}

	values.push(value->subject());

	return {{ entryPoint, values }};
}
