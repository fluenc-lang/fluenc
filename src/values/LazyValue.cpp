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
#include "values/IndexedValue.h"

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

	IteratorStorage iteratorStorage;

	auto sourceEntryPoint = entryPoint
		.withIteratorStorage(&iteratorStorage);

	auto iteratable = source->generate(sourceEntryPoint);

	auto array = dynamic_cast<const ArrayValueGenerator *>(m_generator);

	if (!array)
	{
		throw new std::exception();
	}

	auto iteratableResults = iteratable->build(entryPoint);

	auto branchRatio = iteratableResults.size() / array->size();

	for (auto i = 0u; i < iteratableResults.size(); i++)
	{
		auto arrayIndex = i / branchRatio;

		auto [sourceResultEntryPoint, sourceResultValues] = iteratableResults[i];

		auto sourceBlock = sourceResultEntryPoint.block();

		insertBlock(sourceBlock, function);

		auto sourceValue = sourceResultValues.pop();
		auto targetValue = array->elementAt(arrayIndex);

		auto sourceTupleValue = dynamic_cast<const TupleValue *>(sourceValue);

		if (sourceTupleValue)
		{
			auto sourceTupleValues = sourceTupleValue->values();

			auto resultEntryPoint = ValueHelper::transferValue(sourceResultEntryPoint
				, sourceTupleValues.pop()
				, targetValue
				);

			auto sourceContinuation = sourceTupleValues.require<ExpandableValue>(nullptr);

			auto sourceChain = sourceContinuation->chain();
			auto sourceProvider = sourceContinuation->provider();

			auto sourceContinuationEntryPoint = sourceProvider->withBlock(resultEntryPoint.block());

			for (auto &[chainEntryPoint, _] : sourceChain->build(sourceContinuationEntryPoint, Stack()))
			{
				auto loopTarget = chainEntryPoint.entry();

				linkBlocks(chainEntryPoint.block(), loopTarget->block());
			}
		}
		else
		{
			auto resultEntryPoint = ValueHelper::transferValue(sourceResultEntryPoint
				, sourceValue
				, targetValue
				);

			linkBlocks(resultEntryPoint.block(), exitBlock);
		}
	}

	exitBlock->insertInto(function);

	return entryPoint.withBlock(exitBlock);
}
