#include "index_injector_fragment.hpp"
#include "emitter.hpp"

#include "values/reference_value.hpp"

namespace fluenc::code_generation::fragments
{
	index_injector_fragment::index_injector_fragment(const values::reference_value* index, const fragment* subject)
		: index_(index)
		, subject_(subject)
	{
	}

	std::vector<emitter_result> index_injector_fragment::build(const emitter& visitor, emitter_context context) const
	{
		context.values.push(index_);

		return subject_->build(visitor, context);
	}
}
