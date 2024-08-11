#pragma once

#include <functional>
#include <optional>

#include "entry_point.hpp"

namespace fluenc::code_generation
{
	struct emitter;

	struct transfer_strategy_base
	{
		virtual std::optional<entry_point> invoke(
			const entry_point& entry_point,
			const base_value* source,
			const base_value* target,
			const emitter& emitter
		) const = 0;
	};

	template <typename TSource, typename TTarget>
	using strategy_t = std::function<entry_point(const entry_point&, const TSource*, const TTarget*, const emitter&)>;

	template <typename TSource, typename TTarget>
	struct transfer_strategy : public transfer_strategy_base
	{
		transfer_strategy(strategy_t<TSource, TTarget> callback)
			: callback_(callback)
		{
		}

		std::optional<entry_point> invoke(
			const entry_point& entry_point,
			const base_value* source,
			const base_value* target,
			const emitter& emitter
		) const override
		{
			auto casted_source = value_cast<const TSource*>(source);

			if (!casted_source)
			{
				return {};
			}

			auto casted_target = value_cast<const TTarget*>(target);

			if (!casted_target)
			{
				return {};
			}

			return callback_(entry_point, casted_source, casted_target, emitter);
		}

	private:
		strategy_t<TSource, TTarget> callback_;
	};

	template <typename TSource, typename TTarget>
	auto strategy_for(strategy_t<TSource, TTarget> callback)
	{
		static transfer_strategy<TSource, TTarget> instance(callback);

		return &instance;
	}
}
