#include "continuation_fragment.hpp"
#include "base_value.hpp"
#include "emitter.hpp"
#include "reference_sink_fragment.hpp"
#include "terminator_fragment.hpp"
#include "type.hpp"
#include "value_helper.hpp"

#include "types/expanded_type.hpp"

#include "values/expanded_value.hpp"

#include <algorithm>
#include <numeric>

namespace fluenc::code_generation::fragments
{
	continuation_fragment::continuation_fragment(const fragment* subject, const fragment* consumer)
		: subject_(subject)
		, consumer_(consumer)
	{
	}

	std::vector<emitter_result> continuation_fragment::build(const emitter& visitor, emitter_context context) const
	{
		using std::begin, std::end;

		std::vector<emitter_result> results;

		fragments::reference_sink_fragment reference_sink(fragments::terminator_fragment::instance());

		for (auto [subject_entry_point, subject_values] : subject_->build(visitor, context))
		{
			for (auto [entry_point, values] : reference_sink.build(visitor, { subject_entry_point, subject_values }))
			{
				auto local_entry_point = entry_point;

				auto tail_call_values = entry_point.values();
				auto iterator_type = entry_point.iterator_type();

				std::vector<const base_type*> types = { iterator_type->iterator_type() };

				std::transform(begin(values), end(values), back_inserter(types), [](auto value) {
					return value->type();
				});

				std::vector<const base_value*> cloned;

				std::transform(begin(values), end(values), back_inserter(cloned), [&](auto value) {
					return value->clone(local_entry_point, clone_strategy::value);
				});

				auto next = value_helper::extract_values<values::expanded_value>(values);

				auto is_array = accumulate(begin(next), end(next), next.size() > 0, [](auto accumulated, auto value) {
					return accumulated && value->is_array();
				});

				auto value = new values::
					expanded_value(is_array, types::expanded_type::get(types), entry_point, consumer_, next, cloned);

				results.push_back({ entry_point, value });
			}
		}

		return results;
	}
}
