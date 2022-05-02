#include "ValueHelper.h"
#include "IteratorStorage.h"
#include "IRBuilderEx.h"

#include "types/IteratorType.h"
#include "types/Int64Type.h"

#include "values/LazyValue.h"
#include "values/ILazyValueGenerator.h"
#include "values/IIteratable.h"
#include "values/ArrayValueGenerator.h"
#include "values/TupleValue.h"
#include "values/ExpandableValue.h"
#include "values/ScalarValue.h"

LazyValue::LazyValue(const ILazyValueGenerator *generator)
	: m_generator(generator)
{
}

const Type *LazyValue::type() const
{
	return m_generator->type();
}

const BaseValue *LazyValue::clone(const EntryPoint &entryPoint) const
{
	auto cloned = m_generator->clone(entryPoint);

	return new LazyValue(cloned);
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

	auto exitBlock = llvm::BasicBlock::Create(*context, "exit");
	auto targetEntryBlock = llvm::BasicBlock::Create(*context, "targetEntry");

	IteratorStorage iteratorStorage;

	auto sourceEntryPoint = entryPoint
		.withIteratorStorage(&iteratorStorage);

	auto targetEntryPoint = entryPoint
		.withBlock(targetEntryBlock);

	auto iteratable = source->generate(sourceEntryPoint);
	auto array = m_generator->generate(sourceEntryPoint);

	auto iteratableResults = iteratable->build(entryPoint);
	auto arrayResults = array->build(targetEntryPoint);

	auto branchRatio = iteratableResults.size() / arrayResults.size();

	for (auto i = 0u; i < iteratableResults.size(); i++)
	{
		auto targetIndex = i / branchRatio;

		auto [sourceResultEntryPoint, sourceResultValues] = iteratableResults[i];
		auto [targetResultEntryPoint, targetResultValues] = arrayResults[targetIndex];

		auto sourceBlock = sourceResultEntryPoint.block();
		auto targetBlock = targetResultEntryPoint.block();

		insertBlock(sourceBlock, function);
		insertBlock(targetBlock, function);

		linkBlocks(sourceBlock, targetEntryBlock);

		auto sourceValue = sourceResultValues.pop();
		auto targetValue = targetResultValues.pop();

		if (auto targetTupleValue = dynamic_cast<const TupleValue *>(targetValue))
		{
			auto sourceTupleValue = dynamic_cast<const TupleValue *>(sourceValue);

			if (!sourceTupleValue)
			{
				continue;
			}

			auto targetTupleValues = targetTupleValue->values();
			auto sourceTupleValues = sourceTupleValue->values();

			auto resultEntryPoint = ValueHelper::transferValue(targetResultEntryPoint
				, sourceTupleValues.pop()
				, targetTupleValues.pop()
				);

			auto targetContinuation = targetTupleValues.require<ExpandableValue>(nullptr);
			auto sourceContinuation = sourceTupleValues.require<ExpandableValue>(nullptr);

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

				linkBlocks(chainEntryPoint.block(), loopTarget->block());
			}
		}
		else
		{
			auto resultEntryPoint = ValueHelper::transferValue(targetResultEntryPoint, sourceValue, targetValue);

			linkBlocks(resultEntryPoint.block(), exitBlock);
		}
	}

	insertBlock(targetEntryBlock, function);

	exitBlock->insertInto(function);

	return entryPoint.withBlock(exitBlock);
}
