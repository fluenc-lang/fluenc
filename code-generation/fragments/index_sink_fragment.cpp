#include "index_sink_fragment.hpp"
#include "emitter.hpp"

#include "values/indexed_value.hpp"

namespace fluenc::code_generation::fragments
{
	index_sink_fragment::index_sink_fragment(size_t index, const fragment* consumer)
		: index_(index)
		, consumer_(consumer)
	{
	}

	std::vector<emitter_result> index_sink_fragment::build(const emitter& visitor, emitter_context context) const
	{
		auto indexed = new values::indexed_value(index_, context.values.pop());

		context.values.push(indexed);

		return consumer_->build(visitor, context);
	}
}
