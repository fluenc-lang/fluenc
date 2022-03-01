#include "ArrayValue.h"
#include "TypedValue.h"
#include "TupleValue.h"
#include "ReferenceValue.h"
#include "ExpandableValue.h"
#include "IRBuilderEx.h"
#include "LazyValue.h"
#include "ValueHelper.h"

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

std::vector<DzResult> ArrayValue::build(const EntryPoint &entryPoint, const Stack &values) const
{
	auto block = entryPoint.block();

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

	std::vector<DzResult> results;

	for (auto [elementEntryPoint, elementValues] : m_values)
	{
		elementValues.push(new TypedValue { indexType, index });

		auto arrayBlock = llvm::BasicBlock::Create(*context);

		linkBlocks(block, arrayBlock);

		auto iteratorEntryPoint = entryPoint
			.withBlock(arrayBlock)
			.withName("__array")
			.markEntry()
			;

		for (auto &[resultEntryPoint, resultValues] : m_iterator->build(iteratorEntryPoint, elementValues))
		{
			auto forwardedValues = values;

			for (auto &value : resultValues)
			{
				forwardedValues.push(value);
			}

			results.push_back({ resultEntryPoint, forwardedValues });
		}
	}

	return results;
}

EntryPoint ArrayValue::assignFrom(const EntryPoint &entryPoint, const ArrayValue *source) const
{
	auto block = entryPoint.block();

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

	for (auto &[sourceEntryPoint, sourceValues] : source->build(entryPoint, Stack()))
	{
		auto sourceBlock = sourceEntryPoint.block();

		insertBlock(sourceBlock, function);

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

				insertBlock(targetBlock, function);

				auto targetValue = targetValues.pop();

				if (auto targetTupleValue = dynamic_cast<const TupleValue *>(targetValue))
				{
					auto sourceTupleValue = dynamic_cast<const TupleValue *>(sourceValue);

					if (!sourceTupleValue)
					{
						linkBlocks(targetBlock, targetIteratorEntry);
					}
					else
					{
						auto targetTupleValues = targetTupleValue->values();
						auto sourceTupleValues = sourceTupleValue->values();

						auto targetAddress = targetTupleValues.require<ReferenceValue>();
						auto targetContinuation = targetTupleValues.require<ExpandableValue>();

						auto sourceAddress = sourceTupleValues.require<ReferenceValue>();
						auto sourceContinuation = sourceTupleValues.require<ExpandableValue>();

						IRBuilderEx builder(targetEntryPoint);

						auto load = builder.createLoad(*sourceAddress);

						builder.createStore(load, *targetAddress);

						auto sourceChain = sourceContinuation->chain();
						auto targetChain = targetContinuation->chain();

						auto sourceProvider = sourceContinuation->provider();
						auto targetProvider = targetContinuation->provider();

						auto sourceContinuationEntryPoint = sourceProvider->withBlock(targetBlock);
						auto targetContinuationEntryPoint = targetProvider->withBlock(targetBlock);

						targetChain->build(targetContinuationEntryPoint, Stack());

						for (auto &[chainEntryPoint, _] : sourceChain->build(sourceContinuationEntryPoint, Stack()))
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

						auto load = builder.createLoad(*sourceAddress);

						builder.createStore(load, *targetAddress);

						linkBlocks(targetEntryPoint.block(), exitBlock);
					}
				}
			}
		}
	}

	exitBlock->insertInto(function);

	return entryPoint.withBlock(exitBlock);
}

EntryPoint ArrayValue::assignFrom(const EntryPoint &entryPoint, const LazyValue *source) const
{
	auto block = entryPoint.block();

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

	for (auto &[sourceEntryPoint, sourceValues] : source->build(block, Stack()))
	{
		auto sourceBlock = sourceEntryPoint.block();

		insertBlock(sourceBlock, function);

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

				insertBlock(targetBlock, function);

				auto targetValue = targetValues.pop();

				if (auto targetTupleValue = dynamic_cast<const TupleValue *>(targetValue))
				{
					auto sourceTupleValue = dynamic_cast<const TupleValue *>(sourceValue);

					if (!sourceTupleValue)
					{
						linkBlocks(targetBlock, sourceBlock);
					}
					else
					{
						auto targetTupleValues = targetTupleValue->values();
						auto sourceTupleValues = sourceTupleValue->values();

						ValueHelper::transferValue(targetEntryPoint
							, sourceTupleValues.pop()
							, targetTupleValues.pop()
							);

						auto targetContinuation = targetTupleValues.require<ExpandableValue>();
						auto sourceContinuation = sourceTupleValues.require<ExpandableValue>();

						auto sourceChain = sourceContinuation->chain();
						auto targetChain = targetContinuation->chain();

						auto sourceProvider = sourceContinuation->provider();
						auto targetProvider = targetContinuation->provider();

						auto sourceContinuationEntryPoint = sourceProvider->withBlock(targetBlock);
						auto targetContinuationEntryPoint = targetProvider->withBlock(targetBlock);

						targetChain->build(targetContinuationEntryPoint, Stack());

						for (auto &[chainEntryPoint, _] : sourceChain->build(sourceContinuationEntryPoint, Stack()))
						{
							auto loopTarget = chainEntryPoint.entry();

							linkBlocks(targetBlock, loopTarget->block());
						}
					}
				}
				else if (auto sourceTupleValue = dynamic_cast<const TupleValue *>(sourceValue))
				{
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
					ValueHelper::transferValue(targetEntryPoint, sourceValue, targetValue);

					linkBlocks(targetBlock, exitBlock);
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
