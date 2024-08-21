#include "index_repeater_fragment.hpp"
#include "emitter.hpp"
#include "ir_builder.hpp"

#include "values/reference_value.hpp"
#include "values/scalar_value.hpp"

#include "types/int64_type.hpp"

namespace fluenc::code_generation::fragments
{
	index_repeater_fragment::index_repeater_fragment(const fragment* iteratable)
		: subject_(iteratable)
	{
	}

	std::vector<emitter_result> index_repeater_fragment::build(const emitter& visitor, emitter_context context) const
	{
		auto index = require<values::scalar_value>(context.values, nullptr);
		auto alloc = context.entryPoint.alloc(types::int64_type::instance());

		ir_builder builder(context.entryPoint);

		builder.create_store(index, alloc);

		context.values.push(alloc);

		return subject_->build(visitor, context);
	}
}
