#include "iterator_generator.hpp"
#include "emitter.hpp"
#include "utility.hpp"

#include "fragments/stack_segment_fragment.hpp"

namespace fluenc::code_generation::generators
{
	using strategy_result_t = std::tuple<iterator_generator::strategy_t, std::vector<emitter_result>>;

	strategy_result_t exec(const iterator_generator::use_rendered& strategy, const emitter& emitter, emitter_context context)
	{
		UNUSED(emitter);

		strategy.ep->set_parent(context.entryPoint);

		link_blocks(context.entryPoint.block(), strategy.ep->block());

		std::vector<emitter_result> results;

		for (auto& [result_entry_point, result_values] : strategy.results)
		{
			auto forwarded_entry_point = result_entry_point.with_locals(context.entryPoint.locals());

			results.push_back({ forwarded_entry_point, result_values });
		}

		return { strategy.next, results };
	}

	strategy_result_t exec(const iterator_generator::create_new& strategy, const emitter& emitter, emitter_context context)
	{
		auto locals = context.entryPoint.locals();

		for (auto& [key, value] : strategy.ep->locals())
		{
			locals[key] = value;
		}

		auto ep = context.entryPoint.with_locals(locals);

		return { strategy, strategy.node->build(emitter, { ep, value_stack() }) };
	}

	iterator_generator::iterator_generator(
		const base_type* type,
		const std::vector<emitter_result>& results,
		entry_point* preliminary_entry_point,
		fragments::stack_segment_fragment* subject,
		const entry_point& captured_entry_point
	)
		: type_(type)
		, results_(results)
		, strategy_(use_rendered {
			  .results = results,
			  .ep = preliminary_entry_point,
			  .next =
				  create_new {
					  .node = subject,
					  .ep = new entry_point(captured_entry_point),
				  },
		  })
	{
	}

	std::vector<emitter_result> iterator_generator::generate(
		const emitter& visitor,
		emitter_context context,
		generation_mode mode
	) const
	{
		if (mode == generation_mode::dry_run)
		{
			return results_;
		}

		std::vector<emitter_result> results;

		std::tie(strategy_, results) = std::visit(
			[&](auto& strategy) -> strategy_result_t {
				return exec(strategy, visitor, context);
			},
			strategy_
		);

		return results;
	}

	const base_generator* iterator_generator::clone(const entry_point& entryPoint, clone_strategy strategy) const
	{
		UNUSED(entryPoint);
		UNUSED(strategy);

		return this;
	}

	const base_type* iterator_generator::type() const
	{
		return type_;
	}
}
