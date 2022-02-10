#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

#include "DzArrayElement.h"
#include "DzArrayContinuation.h"
#include "IRBuilderEx.h"

#include "types/IteratorType.h"

#include "values/TypedValue.h"
#include "values/ExpandableValue.h"
#include "values/TupleValue.h"
#include "values/TaintedValue.h"

DzArrayElement::DzArrayElement(const Type *iteratorType, size_t index, DzValue *next)
	: m_iteratorType(iteratorType)
	, m_index(index)
	, m_next(next)
{
}

std::vector<DzResult> DzArrayElement::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();
	auto &module = entryPoint.module();

	auto block = entryPoint.block();
	auto function = entryPoint.function();

	auto dataLayout = module->getDataLayout();

	auto value = values.pop();
	auto index = values.require<TypedValue>();

	if (m_next)
	{
		auto valuesIfFalse = values;
		auto valuesIfTrue = values;

		block->insertInto(function);

		auto indexType = index->type();
		auto storageType = indexType->storageType(*context);

		auto ifTrue = llvm::BasicBlock::Create(*context);
		auto ifFalse = llvm::BasicBlock::Create(*context);

		IRBuilderEx builder(entryPoint);

		auto indexLoad = builder.createLoad(*index, "index");
		auto indexConstant = llvm::ConstantInt::get(storageType, m_index);

		auto comparison =  builder.createCmp(llvm::CmpInst::Predicate::ICMP_EQ, indexLoad, indexConstant);

		builder.createCondBr(comparison , ifTrue, ifFalse);

		auto epIfFalse = entryPoint
			.withBlock(ifFalse);

		auto epIfTrue = entryPoint
			.withBlock(ifTrue);

		valuesIfFalse.push(index);

		auto continuation = new ExpandableValue(m_iteratorType
			, entryPoint
			, new DzArrayContinuation(*index)
			);

		auto tuple = new TupleValue(m_iteratorType
			, { continuation, value }
			);

		valuesIfTrue.push(tuple);

		auto resultsIfFalse = m_next->build(epIfFalse, valuesIfFalse);

		std::vector<DzResult> result = {{ epIfTrue, valuesIfTrue }};

		result.insert(end(result), begin(resultsIfFalse), end(resultsIfFalse));

		return result;
	}

	values.push(value);

	return {{ entryPoint, values }};
}
