#ifndef ITRANSFERSTRATEGY_H
#define ITRANSFERSTRATEGY_H

#include <optional>
#include <numeric>
#include <functional>

#include "EntryPoint.h"

class Emitter;

struct ITransferStrategy
{
	virtual std::optional<EntryPoint> invoke(const EntryPoint &entryPoint, const BaseValue *source, const BaseValue *target, const Emitter &emitter) const = 0;
};

template<typename TSource, typename TTarget>
using strategy_t = std::function<EntryPoint(const EntryPoint &, const TSource *, const TTarget *, const Emitter &)>;

template<typename TSource, typename TTarget>
class TransferStrategy : public ITransferStrategy
{
	public:
		TransferStrategy(strategy_t<TSource, TTarget> callback)
			: m_callback(callback)
		{
		}

		std::optional<EntryPoint> invoke(const EntryPoint &entryPoint, const BaseValue *source, const BaseValue *target, const Emitter &emitter) const override
		{
			auto castedSource = value_cast<const TSource *>(source);

			if (!castedSource)
			{
				return {};
			}

			auto castedTarget = value_cast<const TTarget *>(target);

			if (!castedTarget)
			{
				return {};
			}

			return m_callback(entryPoint, castedSource, castedTarget, emitter);
		}

	private:
		strategy_t<TSource, TTarget> m_callback;
};

template<typename TSource, typename TTarget>
auto strategyFor(strategy_t<TSource, TTarget> callback)
{
	static TransferStrategy<TSource, TTarget> instance(callback);

	return &instance;
}

#endif // ITRANSFERSTRATEGY_H
