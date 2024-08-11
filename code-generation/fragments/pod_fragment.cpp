#include "pod_fragment.hpp"
#include "emitter.hpp"

namespace fluenc::code_generation::fragments
{
	pod_fragment::pod_fragment(fragment* subject, const value_stack& output_values)
		: subject_(subject)
		, output_values_(output_values)
	{
	}

	std::vector<emitter_result> pod_fragment::build(const emitter& visitor, emitter_context context) const
	{
		std::vector<emitter_result> results;

		for (auto& [call_entry_point, call_values] : subject_->build(visitor, context))
		{
			auto forwarded_values = output_values_;

			for (auto& value : call_values)
			{
				forwarded_values.push(value);
			}

			results.push_back({ call_entry_point, forwarded_values });
		}

		return results;
	}
}
