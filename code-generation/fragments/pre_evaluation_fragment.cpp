#include "pre_evaluation_fragment.hpp"
#include "emitter.hpp"
#include "value_stack.hpp"

#include "values/expanded_lazy_value.hpp"
#include "values/pre_lazy_value.hpp"

namespace fluenc::code_generation::fragments
{
	pre_evaluation_fragment::pre_evaluation_fragment(const fragment* consumer)
		: consumer_(consumer)
	{
	}

	std::vector<emitter_result> pre_digest_depth(const emitter& visitor, const entry_point& entryPoint, value_stack values)
	{
		for (auto i = 0u; i < values.size(); i++)
		{
			auto value = values.pop();

			if (auto lazy = value_cast<const values::expanded_lazy_value*>(value))
			{
				std::vector<emitter_result> results;

				for (auto& [result_entry_point, result_values] :
					 lazy->subject()->generate(visitor, { entryPoint, value_stack() }))
				{
					auto forwarded_values = values;

					for (auto& result_value : result_values)
					{
						forwarded_values.push(result_value);
					}

					for (auto& result : pre_digest_depth(visitor, result_entry_point, forwarded_values))
					{
						results.push_back(result);
					}
				}

				return results;
			}

			std::vector<emitter_result> results;

			for (auto& [result_entry_point, result_values] : pre_digest_depth(visitor, entryPoint, values))
			{
				std::vector<const base_value*> forwarded_values;

				for (auto result_value : result_values)
				{
					forwarded_values.push_back(result_value);
				}

				forwarded_values.push_back(value);

				results.push_back({ result_entry_point, forwarded_values });
			}

			return results;
		}

		return { { entryPoint, values } };
	}

	std::vector<emitter_result> pre_evaluation_fragment::build(const emitter& visitor, emitter_context context) const
	{
		std::vector<emitter_result> results;

		for (auto& [result_entry_point, result_values] : pre_digest_depth(visitor, context.entryPoint, context.values))
		{
			for (auto& result : consumer_->build(visitor, { result_entry_point, result_values }))
			{
				results.push_back(result);
			}
		}

		return results;
	}
}
