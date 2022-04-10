#include "values/LazyValue.h"
#include "values/ILazyValueGenerator.h"
#include "values/IIteratable.h"
#include "values/ArrayValueGenerator.h"
#include "values/TupleValue.h"
#include "values/ExpandableValue.h"
#include "ValueHelper.h"
#include "IteratorStorage.h"

#include "types/IteratorType.h"

LazyValue::LazyValue(ILazyValueGenerator *generator)
	: m_generator(generator)
{
}

const Type *LazyValue::type() const
{
	return m_generator->type();
}

const BaseValue *LazyValue::clone(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return this;
}

const IIteratable *LazyValue::generate(const EntryPoint &entryPoint) const
{
	return m_generator->generate(entryPoint);
}

EntryPoint LazyValue::assignFrom(const EntryPoint &entryPoint, const LazyValue *source) const
{
	auto block = entryPoint.block();

	auto &context = entryPoint.context();
	auto &module = entryPoint.module();

	auto function = entryPoint.function();

	insertBlock(block, function);

	auto dataLayout = module->getDataLayout();

	auto exitBlock = llvm::BasicBlock::Create(*context);

	IteratorStorage iteratorStorage;

	auto generatorEntryPoint = entryPoint
		.withIteratorStorage(&iteratorStorage);

	auto iteratable = source->generate(generatorEntryPoint);
	auto array = m_generator->generate(generatorEntryPoint);

	for (auto &[sourceEntryPoint, sourceValues] : iteratable->build(entryPoint))
	{
		auto sourceBlock = sourceEntryPoint.block();

		insertBlock(sourceBlock, function);

		auto sourceValue = sourceValues.pop();

		for (auto &[targetEntryPoint, targetValues] : array->build(sourceEntryPoint))
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

	exitBlock->insertInto(function);

	return entryPoint.withBlock(exitBlock);
}
