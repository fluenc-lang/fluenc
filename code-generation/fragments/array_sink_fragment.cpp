#include "array_sink_fragment.hpp"
#include "ast.hpp"
#include "emitter.hpp"
#include "index_sink_fragment.hpp"
#include "reference_sink_fragment.hpp"
#include "terminator_fragment.hpp"

#include "generators/array_iterator_generator.hpp"

#include "values/pre_lazy_value.hpp"

#include <numeric>

namespace fluenc::code_generation::fragments
{
	array_sink_fragment::array_sink_fragment(
		const std::vector<indexed<expression_t>>& elements,
		const std::shared_ptr<peg::Ast> ast,
		const fragment* consumer
	)
		: elements_(elements)
		, ast_(ast)
		, consumer_(consumer)
	{
	}

	std::vector<emitter_result> array_sink_fragment::build(const emitter& visitor, emitter_context context) const
	{
		auto array_contents = std::accumulate(
			rbegin(elements_),
			rend(elements_),
			std::vector<emitter_result> { { context.entryPoint, value_stack_t() } },
			[&](auto accumulated_results, fluenc::indexed<expression_t> expression) -> std::vector<emitter_result> {
				index_sink_fragment index_sink(expression.index, terminator_fragment::instance());
				reference_sink_fragment reference_sink(&index_sink);

				std::vector<emitter_result> results;

				for (auto [accumulated_entry_point, accumulated_values] : accumulated_results)
				{
					for (auto& [entry_point, values] :
						 accept(expression.value, visitor, { accumulated_entry_point, accumulated_values }))
					{
						for (auto& result : reference_sink.build(visitor, { entry_point, values }))
						{
							results.push_back(result);
						}
					}
				}

				return results;
			}
		);

		auto generator = new generators::array_iterator_generator(array_contents, ast_, size(elements_));
		auto lazy = new values::pre_lazy_value(generator, context.entryPoint);

		context.values.push(lazy);

		std::vector<emitter_result> results;

		for (auto& [array_entry_point, _] : array_contents)
		{
			for (auto& result : consumer_->build(visitor, { array_entry_point, context.values }))
			{
				results.push_back(result);
			}
		}

		return results;
	}
}
