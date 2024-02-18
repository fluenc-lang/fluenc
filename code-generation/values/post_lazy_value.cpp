#include "post_lazy_value.hpp"
#include "emitter.hpp"
#include "fragment.hpp"
#include "utility.hpp"

namespace fluenc::code_generation::values
{
	using strategy_result_t = std::tuple<post_lazy_value::strategy_t, std::vector<emitter_result>>;

	strategy_result_t exec(const post_lazy_value::use_rendered& strategy, const emitter& emitter, emitter_context context)
	{
		UNUSED(emitter);

		link_blocks(context.entryPoint.block(), strategy.ep->block());

		std::vector<emitter_result> results;

		for (auto& [result_entry_point, result_values] : strategy.results)
		{
			auto forwarded_entry_point = result_entry_point.with_locals(context.entryPoint.locals());

			results.push_back({ forwarded_entry_point, result_values });
		}

		return { strategy.next, results };
	}

	strategy_result_t exec(const post_lazy_value::create_new& strategy, const emitter& emitter, emitter_context context)
	{
		return { strategy, strategy.node->build(emitter, { context.entryPoint, strategy.values }) };
	}

	post_lazy_value::post_lazy_value(
		const fragment* node,
		const base_type* type,
		const entry_point& entryPoint,
		const value_stack& values,
		const std::vector<emitter_result>& results
	)
		: type_(type)
		, strategy_(use_rendered {
			  .results = results,
			  .ep = new entry_point(entryPoint),
			  .next = {
			  	.node = node,
			  	.values = values,
			  },
		  })
	{
	}

	value_id post_lazy_value::id() const
	{
		return value_id::post_lazy;
	}

	const base_type* post_lazy_value::type() const
	{
		return type_;
	}

	const base_value* post_lazy_value::clone(const entry_point& entryPoint, clone_strategy strategy) const
	{
		UNUSED(entryPoint);
		UNUSED(strategy);

		return this;
	}

	std::vector<emitter_result> post_lazy_value::generate(const emitter& emitter, emitter_context context) const
	{
		std::vector<emitter_result> results;

		std::tie(strategy_, results) = std::visit(
			[&](auto& strategy) -> strategy_result_t {
				return exec(strategy, emitter, context);
			},
			strategy_
		);

		return results;
	}
}
