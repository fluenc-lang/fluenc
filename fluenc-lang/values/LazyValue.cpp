#include <unordered_map>

#include "ValueHelper.h"
#include "FunctionHelper.h"

#include "types/ArrayType.h"

#include "values/LazyValue.h"
#include "values/ILazyValueGenerator.h"
#include "values/IIteratable.h"
#include "values/ArrayValueGenerator.h"
#include "values/IteratorValueGenerator.h"
#include "values/TupleValue.h"
#include "values/ExpandableValue.h"
#include "values/Iterator.h"

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

			auto continuationEntryPoint = provider->withBlock(entryPoint.block());

			Emitter analyzer;

			auto chainResult = chain->accept(analyzer, { continuationEntryPoint, Stack() });

			auto [_, chainValues] = *chainResult.begin();

			return getElementType(accumulated, *provider, chainValues);
		}

		if (auto expanded = dynamic_cast<const ExpandedValue *>(value))
		{
			return { expanded->isArray(), expanded->type() };
		}

		return { accumulated.first, value->type() };
	});
}

std::vector<DzResult> expandIterator(const Emitter &emitter, DefaultVisitorContext context)
{
	auto digestDepth = [&](const EntryPoint &entryPoint, Stack values, auto &recurse) -> std::vector<DzResult>
	{
		for (auto i = 0u; i < values.size(); i++)
		{
			auto value = values.pop();

			if (auto iterator = dynamic_cast<const Iterator *>(value))
			{
				std::vector<DzResult> results;

				for (auto &[resultEntryPoint, resultValues] : iterator->generate(emitter, { entryPoint, Stack() }))
				{
					auto forwardedValues = values;

					for (auto &resultValue : resultValues)
					{
						forwardedValues.push(resultValue);
					}

					for (auto &result : recurse(resultEntryPoint, forwardedValues, recurse))
					{
						results.push_back(result);
					}
				}

				return results;
			}

			std::vector<DzResult> results;

			for (auto &[resultEntryPoint, resultValues] : recurse(entryPoint, values, recurse))
			{
				std::vector<const BaseValue *> forwardedValues;

				for (auto resultValue : resultValues)
				{
					forwardedValues.push_back(resultValue);
				}

				forwardedValues.push_back(value);

				results.push_back({ resultEntryPoint, forwardedValues });
			}

			return results;
		}

		return {{ entryPoint, values }};
	};

	std::vector<DzResult> results;

	for (auto &result : digestDepth(context.entryPoint, context.values, digestDepth))
	{
		results.push_back(result);
	}

	return results;
}

const Type *LazyValue::type() const
{
	auto context = m_entryPoint->context();

	auto block = createBlock(context);
	auto alloc = createBlock(context);

	linkBlocks(alloc, block);

	auto entryPoint = (*m_entryPoint)
		.withBlock(block)
		.withAlloc(alloc);

	auto iteratable = m_generator->generate(entryPoint, GenerationMode::DryRun);

	Emitter analyzer;

	std::vector<DzResult> results;

	for (auto &[resultEntryPoint, resultValues] : iteratable->accept(analyzer, { entryPoint, Stack() }))
	{
		auto resultBlock = createBlock(context);

		for (auto &result : expandIterator(analyzer, { resultEntryPoint.withBlock(resultBlock), resultValues }))
		{
			results.push_back(result);
		}
	}

	std::map<int, const Type *> typesByIndex;

	for (auto &[resultEntryPoint, resultValues] : results)
	{
		auto resultBlock = createBlock(context);

		auto [isArrayCompatible, type] = getElementType({ true, nullptr }, resultEntryPoint.withBlock(resultBlock), resultValues);

		if (!isArrayCompatible)
		{
			return m_generator->type();
		}

		auto index = resultEntryPoint.index();

		if (auto existing = typesByIndex.find(index); existing != end(typesByIndex))
		{
			if (existing->second->compatibility(type, entryPoint) != 0)
			{
				return m_generator->type();
			}
		}

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

const BaseValue *LazyValue::clone(const EntryPoint &entryPoint, CloneStrategy strategy) const
{
	auto cloned = m_generator->clone(entryPoint, strategy);

	return new LazyValue(cloned, entryPoint);
}

const BaseValue *LazyValue::forward(size_t id) const
{
	auto forwarded = m_generator->forward(id);

	return new LazyValue(forwarded, *m_entryPoint);
}

const IIteratable *LazyValue::generate(const EntryPoint &entryPoint) const
{
	return m_generator->generate(entryPoint, GenerationMode::Regular);
}

EntryPoint LazyValue::assignFrom(const EntryPoint &entryPoint, const BaseValue *source, const Emitter &emitter) const
{
	auto context = entryPoint.context();

	auto targetIteratable = m_generator->generate(entryPoint, GenerationMode::Regular);

	std::vector<DzResult> targetResults;

	for (auto &[resultEntryPoint, resultValues] : targetIteratable->accept(emitter, { entryPoint, Stack() }))
	{
		for (auto &result : expandIterator(emitter, { resultEntryPoint, resultValues }))
		{
			targetResults.push_back(result);
		}
	}

	std::unordered_map<uint32_t, const BaseValue *> indexedResults;

	auto targetExitBlock = llvm::BasicBlock::Create(*context, "targetExit");

	for (auto [resultEntryPoint, resultValues] : targetResults)
	{
		auto values =  ValueHelper::extractValues<BaseValue>(resultValues);

		if (values.size() <= 0)
		{
			throw new std::exception();
		}

		indexedResults.insert({ resultEntryPoint.index(), values[0] });

		linkBlocks(resultEntryPoint.block(), targetExitBlock);
	}

	auto sourceEntryPoint = entryPoint
		.withBlock(targetExitBlock);

	auto sourceExitBlock = llvm::BasicBlock::Create(*context, "sourceExit");

	std::vector<DzResult> sourceResults = {{ sourceEntryPoint, source }};

	for (auto i = 0u; i < sourceResults.size(); i++)
	{
		auto [sourceResultEntryPoint, sourceResultValues] = sourceResults[i];

		auto sourceValue = sourceResultValues.pop();
		auto targetValue = indexedResults[sourceResultEntryPoint.index()];

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

			for (auto &[chainEntryPoint, chainValues] : sourceChain->accept(emitter, { sourceContinuationEntryPoint, Stack() }))
			{
				auto sourceValue = chainValues.require<ExpandedValue>(nullptr);

				auto loopTarget = FunctionHelper::findTailCallTarget(&chainEntryPoint, Stack(sourceValue));
				auto resultEntryPoint = ValueHelper::transferValue(chainEntryPoint, sourceValue, sourceContinuation, emitter);

				auto loopTargetEntry = loopTarget->entry();

				linkBlocks(resultEntryPoint.block(), loopTargetEntry->block());
			}
		}
		else
		{
			auto resultEntryPoint = ValueHelper::transferValue(sourceResultEntryPoint
				, sourceValue
				, targetValue
				, emitter
				);

			linkBlocks(resultEntryPoint.block(), sourceExitBlock);
		}
	}

	return entryPoint.withBlock(sourceExitBlock);
}

EntryPoint LazyValue::assignFrom(const EntryPoint &entryPoint, const LazyValue *source, const Emitter &emitter) const
{
	auto context = entryPoint.context();

	auto sourceIteratable = source->generate(entryPoint);
	auto targetIteratable = m_generator->generate(entryPoint, GenerationMode::Regular);

	std::vector<DzResult> targetResults;

	for (auto &[resultEntryPoint, resultValues] : targetIteratable->accept(emitter, { entryPoint, Stack() }))
	{
		for (auto &result : expandIterator(emitter, { resultEntryPoint, resultValues }))
		{
			targetResults.push_back(result);
		}
	}

	std::unordered_map<uint32_t, const BaseValue *> indexedResults;

	auto targetExitBlock = llvm::BasicBlock::Create(*context, "targetExit");

	for (auto [resultEntryPoint, resultValues] : targetResults)
	{
		auto values =  ValueHelper::extractValues<BaseValue>(resultValues);

		if (values.size() <= 0)
		{
			throw new std::exception();
		}

		indexedResults.insert({ resultEntryPoint.index(), values[0] });

		linkBlocks(resultEntryPoint.block(), targetExitBlock);
	}

	auto sourceEntryPoint = entryPoint
		.withBlock(targetExitBlock);

	auto sourceExitBlock = llvm::BasicBlock::Create(*context, "sourceExit");

	std::vector<DzResult> sourceResults;

	for (auto &[resultEntryPoint, resultValues] : sourceIteratable->accept(emitter, { sourceEntryPoint, Stack() }))
	{
		for (auto &result : expandIterator(emitter, { resultEntryPoint, resultValues }))
		{
			sourceResults.push_back(result);
		}
	}

	for (auto i = 0u; i < sourceResults.size(); i++)
	{
		auto [sourceResultEntryPoint, sourceResultValues] = sourceResults[i];

		auto sourceValue = sourceResultValues.pop();
		auto targetValue = indexedResults[sourceResultEntryPoint.index()];

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

			for (auto &[chainEntryPoint, chainValues] : sourceChain->accept(emitter, { sourceContinuationEntryPoint, Stack() }))
			{
				auto sourceValue = chainValues.require<ExpandedValue>(nullptr);

				auto loopTarget = FunctionHelper::findTailCallTarget(&chainEntryPoint, Stack(sourceValue));
				auto resultEntryPoint = ValueHelper::transferValue(chainEntryPoint, sourceValue, sourceContinuation, emitter);

				auto loopTargetEntry = loopTarget->entry();

				linkBlocks(resultEntryPoint.block(), loopTargetEntry->block());
			}
		}
		else
		{
			auto resultEntryPoint = ValueHelper::transferValue(sourceResultEntryPoint
				, sourceValue
				, targetValue
				, emitter
				);

			linkBlocks(resultEntryPoint.block(), sourceExitBlock);
		}
	}

	return entryPoint.withBlock(sourceExitBlock);
}
