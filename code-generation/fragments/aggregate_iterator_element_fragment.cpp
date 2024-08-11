#include "aggregate_iterator_element_fragment.hpp"
#include "ast.hpp"
#include "emitter.hpp"

namespace fluenc::code_generation::fragments
{
	aggregate_iterator_element_fragment::aggregate_iterator_element_fragment(const base_value* value, const fragment* next)
		: value_(value)
		, consumer_(next)
	{
	}

	std::vector<emitter_result> aggregate_iterator_element_fragment::build(const emitter& visitor, emitter_context context) const
	{
		context.values.push(value_);

		std::vector<emitter_result> results;

		fluenc::expansion_node consumer {
			.consumer = fluenc::terminator_node::instance(),
			.ast = {},
		};

		for (auto& [entryPoint, values] : accept(&consumer, visitor, context))
		{
			for (auto& result : consumer_->build(visitor, { entryPoint, values }))
			{
				results.push_back(result);
			}
		}

		return results;
	}
}
