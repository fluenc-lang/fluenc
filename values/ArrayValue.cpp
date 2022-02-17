#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>

#include "ArrayValue.h"
#include "TypedValue.h"
#include "TupleValue.h"
#include "ReferenceValue.h"
#include "ExpandableValue.h"
#include "IRBuilderEx.h"

#include "types/IteratorType.h"
#include "types/Int64Type.h"

ArrayValue::ArrayValue(const EntryPoint &entryPoint
	, const Type *iteratorType
	, const DzValue *iterator
	, const std::vector<DzResult> &values
	)
	: m_entryPoint(new EntryPoint(entryPoint))
	, m_iteratorType(iteratorType)
	, m_iterator(iterator)
	, m_values(values)
{
}

std::vector<DzResult> ArrayValue::build(llvm::BasicBlock *block, const Stack &values) const
{
	auto entryPoint = m_entryPoint->withBlock(block);

	auto &context = entryPoint.context();
	auto &module = entryPoint.module();

	auto function = entryPoint.function();

	block->insertInto(function);

	auto dataLayout = module->getDataLayout();

	auto indexType = Int64Type::instance();
	auto storageType = indexType->storageType(*context);

	auto align = dataLayout.getABITypeAlign(storageType);

	auto zero = llvm::ConstantInt::get(storageType, 0);

	auto index = entryPoint.alloc(storageType);

	auto store = new llvm::StoreInst(zero, index, false, align, block);

	UNUSED(store);

	std::vector<DzResult> results;

	for (auto [elementEntryPoint, elementValues] : m_values)
	{
		auto forwardedValues = values;

		for (auto &value : elementValues)
		{
			forwardedValues.push(value);
		}

		forwardedValues.push(new TypedValue { indexType, index });

		auto arrayBlock = llvm::BasicBlock::Create(*context);

		linkBlocks(block, arrayBlock);

		auto iteratorEntryPoint = entryPoint
			.withBlock(arrayBlock)
			.markEntry()
			;

		for (auto &result : m_iterator->build(iteratorEntryPoint, forwardedValues))
		{
			results.push_back(result);
		}
	}

	return results;
}

EntryPoint ArrayValue::storeInto(llvm::BasicBlock *block, const ArrayValue *target) const
{
	auto entryPoint = m_entryPoint->withBlock(block);

	auto &context = entryPoint.context();
	auto &module = entryPoint.module();

	auto function = entryPoint.function();

	insertBlock(block, function);

	auto dataLayout = module->getDataLayout();

	auto indexType = Int64Type::instance();
	auto storageType = indexType->storageType(*context);

	auto align = dataLayout.getABITypeAlign(storageType);

	auto zero = llvm::ConstantInt::get(storageType, 0);

	auto index = entryPoint.alloc(storageType);

	auto store = new llvm::StoreInst(zero, index, false, align, block);

	UNUSED(store);

	auto exitBlock = llvm::BasicBlock::Create(*context);

	for (auto [sourceElementEntryPoint, sourceElementValues] : target->m_values)
	{
		sourceElementValues.push(new TypedValue { indexType, index });

		auto sourceIteratorEntry = llvm::BasicBlock::Create(*context, "", function);

		linkBlocks(block, sourceIteratorEntry);

		auto sourceIteratorEntryPoint = sourceElementEntryPoint
			.withBlock(sourceIteratorEntry)
			.markEntry()
			;

		for (auto &[sourceEntryPoint, sourceValues] : target->m_iterator->build(sourceIteratorEntryPoint, sourceElementValues))
		{
			auto sourceBlock = sourceEntryPoint.block();

			sourceBlock->insertInto(function);

			auto sourceValue = sourceValues.pop();

			for (auto [targetElementEntryPoint, targetElementValues] : m_values)
			{
				targetElementValues.push(new TypedValue { indexType, index });

				auto targetIteratorEntry = llvm::BasicBlock::Create(*context, "", function);

				linkBlocks(sourceBlock, targetIteratorEntry);

				auto targetIteratorEntryPoint = targetElementEntryPoint
					.withBlock(targetIteratorEntry)
					.markEntry()
					;

				for (auto &[targetEntryPoint, targetValues] : m_iterator->build(targetIteratorEntryPoint, targetElementValues))
				{
					auto targetBlock = targetEntryPoint.block();

					targetBlock->insertInto(function);

					auto targetValue = targetValues.pop();

					if (auto targetTupleValue = dynamic_cast<const TupleValue *>(targetValue))
					{
						auto sourceTupleValue = dynamic_cast<const TupleValue *>(sourceValue);

						if (!sourceTupleValue)
						{
							linkBlocks(targetEntryPoint.block(), targetIteratorEntry);
						}
						else
						{
							auto targetTupleValues = targetTupleValue->values();
							auto sourceTupleValues = sourceTupleValue->values();

							auto targetAddress = targetTupleValues.require<ReferenceValue>();

							auto sourceAddress = sourceTupleValues.require<ReferenceValue>();
							auto continuation = sourceTupleValues.require<ExpandableValue>();

							IRBuilderEx builder(targetEntryPoint);

							auto load = builder.createLoad(*targetAddress);

							builder.createStore(load, *sourceAddress);

							auto chain = continuation->chain();
							auto provider = continuation->provider();

							auto continuationEntryPoint = provider->withBlock(targetBlock);

							for (auto &[chainEntryPoint, _] : chain->build(continuationEntryPoint, Stack()))
							{
								auto loopTarget = chainEntryPoint.entry();

								linkBlocks(targetBlock, loopTarget->block());
							}
						}
					}
					else if (auto targetAddress = dynamic_cast<const ReferenceValue *>(targetValue))
					{
						auto sourceAddress = dynamic_cast<const ReferenceValue *>(sourceValue);

						if (!sourceAddress)
						{
							auto sourceTupleValue = dynamic_cast<const TupleValue *>(sourceValue);

							if (!sourceTupleValue)
							{
								throw new std::exception();
							}

							auto sourceTupleValues = sourceTupleValue->values();

							auto continuation = sourceTupleValues
								.discard()
								.require<ExpandableValue>();

							auto chain = continuation->chain();
							auto provider = continuation->provider();

							auto continuationEntryPoint = provider->withBlock(targetBlock);

							for (auto &[chainEntryPoint, _] : chain->build(continuationEntryPoint, Stack()))
							{
								auto loopTarget = chainEntryPoint.entry();

								linkBlocks(targetBlock, loopTarget->block());
							}
						}
						else
						{
							IRBuilderEx builder(targetEntryPoint);

							auto load = builder.createLoad(*targetAddress);

							builder.createStore(load, *sourceAddress);

							linkBlocks(targetEntryPoint.block(), exitBlock);
						}
					}
				}
			}
		}
	}

	exitBlock->insertInto(function);

	return entryPoint.withBlock(exitBlock);
}

const Type *ArrayValue::type() const
{
	// This is too naive.
	// Need to come up with a way of how to represent the type of arrays.

	return IteratorType::instance();
}

const BaseValue *ArrayValue::clone(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return this;
}
