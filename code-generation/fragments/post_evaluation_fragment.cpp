#include "post_evaluation_fragment.hpp"
#include "emitter.hpp"
#include "pre_evaluation_fragment.hpp"
#include "terminator_fragment.hpp"
#include "value_stack.hpp"

#include "values/expanded_value.hpp"
#include "values/forwarded_value.hpp"
#include "values/post_lazy_value.hpp"
#include "values/tuple_value.hpp"

namespace fluenc::code_generation::fragments
{
	std::vector<emitter_result> post_digest_depth(const emitter& visitor, const entry_point& entryPoint, value_stack values)
	{
		for (auto i = 0u; i < values.size(); i++)
		{
			auto value = values.pop();

			if (auto iterator = value_cast<const values::post_lazy_value*>(value))
			{
				std::vector<emitter_result> results;

				for (auto& [result_entry_point, result_values] :
					 iterator->generate(visitor, { entryPoint, value_stack() }))
				{
					auto forwarded_values = values;

					for (auto& result_value : result_values)
					{
						forwarded_values.push(result_value);
					}

					for (auto& result : post_digest_depth(visitor, result_entry_point, forwarded_values))
					{
						results.push_back(result);
					}
				}

				return results;
			}

			if (auto forwarded = value_cast<const values::forwarded_value*>(value))
			{
				std::vector<emitter_result> results;

				for (auto& [result_entry_point, result_values] : post_digest_depth(visitor, entryPoint, values))
				{
					std::vector<const base_value*> forwarded_values;

					for (auto result_value : result_values)
					{
						forwarded_values.push_back(result_value);
					}

					auto subject = forwarded->subject();

					forwarded_values.push_back(subject);

					results.push_back({ result_entry_point, forwarded_values });
				}

				return results;
			}

			if (auto tuple = value_cast<const values::tuple_value*>(value))
			{
				for (auto& element : tuple->values())
				{
					if (auto expanded = value_cast<const values::expanded_value*>(element))
					{
						std::vector<emitter_result> results;

						auto node = expanded->node();

						auto pre = new pre_evaluation_fragment(terminator_fragment::instance());

						for (auto& [rep, rva] : pre->build(visitor, { entryPoint, expanded->values() }))
						{
							for (auto& [input_entry_point, input_values] : post_digest_depth(visitor, rep, rva))
							{
								for (auto& [result_entry_point, result_values] :
									 node->build(visitor, { input_entry_point, input_values }))
								{
									auto forwarded_entry_point = entryPoint.with_block(result_entry_point.block());

									for (auto& [final_entry_point, final_values] :
										 post_digest_depth(visitor, forwarded_entry_point, result_values))
									{
										auto forwarded_values = values;

										for (auto result_value : final_values)
										{
											forwarded_values.push(result_value);
										}

										results.push_back({ final_entry_point, forwarded_values });
									}
								}
							}
						}

						return results;
					}
				}

				std::vector<emitter_result> results;

				auto digested_results = post_digest_depth(visitor, entryPoint, tuple->values());

				for (auto& [digested_entry_point, digested_values] : digested_results)
				{
					auto digested_tuple = new values::tuple_value({ digested_values.begin(), digested_values.end() });

					for (auto& [result_entry_point, result_values] :
						 post_digest_depth(visitor, digested_entry_point, values))
					{
						std::vector<const base_value*> forwarded_values;

						for (auto result_value : result_values)
						{
							forwarded_values.push_back(result_value);
						}

						forwarded_values.push_back(digested_tuple);

						results.push_back({ result_entry_point, forwarded_values });
					}
				}

				return results;
			}

			std::vector<emitter_result> results;

			for (auto& [result_entry_point, result_values] : post_digest_depth(visitor, entryPoint, values))
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

	std::vector<emitter_result> post_evaluation_fragment::build(const emitter& visitor, emitter_context context) const
	{
		return post_digest_depth(visitor, context.entryPoint, context.values);
	}
}
