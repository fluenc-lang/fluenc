#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

#include "DzArrayElement.h"
#include "DzArrayContinuation.h"

#include "types/IteratorType.h"

#include "values/TypedValue.h"
#include "values/ExpandableValue.h"
#include "values/TupleValue.h"

DzArrayElement::DzArrayElement(size_t index, DzValue *consumer, DzValue *next)
	: m_index(index)
	, m_consumer(consumer)
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

		llvm::IRBuilder<> builder(block);

		auto align = dataLayout.getABITypeAlign(storageType);

		auto indexLoad = new llvm::LoadInst(storageType, *index, "index", false, align, block);
		auto indexConstant = llvm::ConstantInt::get(storageType, m_index);

		auto comparison =  builder.CreateCmp(llvm::CmpInst::Predicate::ICMP_EQ, indexLoad, indexConstant);

		builder.CreateCondBr(comparison , ifTrue, ifFalse);

		auto epIfFalse = entryPoint
			.withBlock(ifFalse);

		auto epIfTrue = entryPoint
			.withBlock(ifTrue);

		valuesIfFalse.push(index);

		auto continuation = new ExpandableValue(entryPoint
			, new DzArrayContinuation(*index)
			);

		auto tuple = new TupleValue({ continuation, value });

		valuesIfTrue.push(tuple);

		auto resultsIfTrue = m_consumer->build(epIfTrue, valuesIfTrue);
		auto resultsIfFalse = m_next->build(epIfFalse, valuesIfFalse);

		std::vector<DzResult> result;

		result.insert(end(result), begin(resultsIfTrue), end(resultsIfTrue));
		result.insert(end(result), begin(resultsIfFalse), end(resultsIfFalse));

		return result;
	}

	values.push(value);

	return m_consumer->build(entryPoint, values);
}
