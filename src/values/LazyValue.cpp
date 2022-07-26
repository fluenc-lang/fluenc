#include "ValueHelper.h"
#include "IteratorStorage.h"
#include "FunctionHelper.h"

#include "types/ArrayType.h"

#include "values/LazyValue.h"
#include "values/ILazyValueGenerator.h"
#include "values/IIteratable.h"
#include "values/ArrayValueGenerator.h"
#include "values/TupleValue.h"
#include "values/ExpandableValue.h"

using ElementType = std::pair<bool, const Type *>;

LazyValue::LazyValue(const ILazyValueGenerator *generator, const EntryPoint &entryPoint)
	: m_generator(generator)
	, m_entryPoint(new EntryPoint(entryPoint))
{
}

ElementType getElementType(ElementType seed, const EntryPoint &entryPoint, Stack values)
{
	return accumulate(values.begin(), values.end(), seed, [&](auto accumulated, auto value) -> ElementType
	{
		if (auto tuple = dynamic_cast<const TupleValue *>(value))
		{
			return getElementType({ false, accumulated.second }, entryPoint, tuple->values());
		}

		if (auto expandable = dynamic_cast<const ExpandableValue *>(value))
		{
			auto chain = expandable->chain();
			auto provider = expandable->provider();

			Emitter emitter;

			auto chainEntryPoint = provider->withBlock(entryPoint.block());
			auto chainResult = chain->accept(emitter, chainEntryPoint, Stack());

			auto [_, chainValues] = *chainResult.begin();

			return getElementType(accumulated, chainEntryPoint, chainValues);
		}

		if (auto expanded = dynamic_cast<const ExpandedValue *>(value))
		{
			return { expanded->isArray(), expanded->type() };
		}

		return { accumulated.first, value->type() };
	});
};

const Type *LazyValue::type() const
{
	auto context = m_entryPoint->context();

	auto block = llvm::BasicBlock::Create(*context);
	auto alloc = llvm::BasicBlock::Create(*context);

	linkBlocks(alloc, block);

	auto iteratorStorage = new IteratorStorage();

	auto entryPoint = (*m_entryPoint)
		.withBlock(block)
		.withAlloc(alloc)
		.withIteratorStorage(iteratorStorage);

	auto iteratable = m_generator->generate(entryPoint);

	Emitter emitter;

	auto results = iteratable->accept(emitter, entryPoint, Stack());

	std::map<int, const Type *> typesByIndex;

	for (auto &[resultEntryPoint, resultValues] : results)
	{
		auto [isArrayCompatible, type] = getElementType({ true, nullptr }, resultEntryPoint, resultValues);

		if (!isArrayCompatible)
		{
			return m_generator->type();
		}

		auto index = resultEntryPoint.index();

		typesByIndex[index] = type;
	}

	std::vector<const Type *> types;

	transform(begin(typesByIndex), end(typesByIndex), back_inserter(types), [](auto pair)
	{
		return pair.second;
	});

	if (types.size() > 1)
	{
		return ArrayType::get(types);
	}

	return types[0];
}

const BaseValue *LazyValue::clone(const EntryPoint &entryPoint) const
{
	auto cloned = m_generator->clone(entryPoint);

	return new LazyValue(cloned, entryPoint);
}

const BaseValue *LazyValue::forward(size_t id) const
{
	auto forwarded = m_generator->forward(id);

	return new LazyValue(forwarded, *m_entryPoint);
}

const IIteratable *LazyValue::generate(const EntryPoint &entryPoint) const
{
	return m_generator->generate(entryPoint);
}

EntryPoint LazyValue::assignFrom(const EntryPoint &entryPoint, const LazyValue *source, const Emitter &emitter) const
{
	auto block = entryPoint.block();

	auto context = entryPoint.context();
	auto module = entryPoint.module();

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

	auto iteratableResults = iteratable->accept(emitter, entryPoint, Stack());

	for (auto i = 0u; i < iteratableResults.size(); i++)
	{
		auto [sourceResultEntryPoint, sourceResultValues] = iteratableResults[i];

		auto sourceBlock = sourceResultEntryPoint.block();

		insertBlock(sourceBlock, function);

		auto sourceValue = sourceResultValues.pop();
		auto targetValue = array->elementAt(sourceResultEntryPoint.index());

		auto sourceTupleValue = dynamic_cast<const TupleValue *>(sourceValue);

		if (sourceTupleValue)
		{
			auto sourceTupleValues = sourceTupleValue->values();

			auto resultEntryPoint = ValueHelper::transferValue(sourceResultEntryPoint
				, sourceTupleValues.pop()
				, targetValue
				, emitter
				);

			auto sourceContinuation = sourceTupleValues.require<ExpandableValue>(nullptr);

			auto sourceChain = sourceContinuation->chain();
			auto sourceProvider = sourceContinuation->provider();

			auto sourceContinuationEntryPoint = sourceProvider->withBlock(resultEntryPoint.block());

			for (auto &[chainEntryPoint, chainValues] : sourceChain->accept(emitter, sourceContinuationEntryPoint, Stack()))
			{
				auto loopTarget = FunctionHelper::findTailCallTarget(&chainEntryPoint, chainValues);

				auto loopTargetEntry = loopTarget->entry();

				linkBlocks(chainEntryPoint.block(), loopTargetEntry->block());
			}
		}
		else
		{
			auto resultEntryPoint = ValueHelper::transferValue(sourceResultEntryPoint
				, sourceValue
				, targetValue
				, emitter
				);

			linkBlocks(resultEntryPoint.block(), exitBlock);
		}
	}

	exitBlock->insertInto(function);

	return entryPoint.withBlock(exitBlock);
}
