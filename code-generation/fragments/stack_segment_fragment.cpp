#include "stack_segment_fragment.hpp"
#include "ast.hpp"
#include "base_value.hpp"
#include "emitter.hpp"
#include "indexed.hpp"
#include "pod_fragment.hpp"
#include "pre_evaluation_fragment.hpp"
#include "reference_sink_fragment.hpp"
#include "terminator_fragment.hpp"
#include "utility.hpp"
#include "value_stack.hpp"

#include "iterators/index_iterator.hpp"

#include "values/indexed_value.hpp"
#include "values/post_lazy_value.hpp"

#include "types/iterator_type.hpp"

#include <algorithm>
#include <numeric>
#include <span>

namespace fluenc::code_generation::fragments
{
	stack_segment_fragment::stack_segment_fragment(
		const std::vector<fluenc::expression_t>& values,
		fragment* call,
		fluenc::expression_t consumer
	)
		: values_(values)
		, call_(call)
		, consumer_(consumer)
	{
	}

	std::vector<emitter_result> stack_segment_fragment::build(const emitter& visitor, emitter_context context) const
	{
		std::vector<emitter_result> result;
		std::vector<emitter_result> input = { { context.entryPoint.with_index(-1), value_stack() } };

		std::vector<fluenc::indexed<fluenc::expression_t>> orderedValues;

		std::span<const base_value* const> v(context.values.begin(), values_.size());

		std::transform(
			begin(values_),
			end(values_),
			fluenc::index_iterator(),
			std::back_inserter(orderedValues),
			[](auto value, auto index) -> fluenc::indexed<fluenc::expression_t> {
				return { index, value };
			}
		);

		auto reference_sink = new reference_sink_fragment(terminator_fragment::instance());
		auto pre_evaluation = new pre_evaluation_fragment(reference_sink);

		auto subject_results = std::
			accumulate(begin(orderedValues), end(orderedValues), input, [&](auto accumulator, auto argument) {
				std::vector<emitter_result> results;

				for (auto& [accumulatorEntryPoint, accumulatorValues] : accumulator)
				{
					auto result = accept(argument.value, visitor, { accumulatorEntryPoint, value_stack() });

					for (auto& [resultEntryPoint, resultValues] : result)
					{
						for (auto& [rep, rva] : pre_evaluation->build(visitor, { resultEntryPoint, resultValues }))
						{
							auto scopedReturnValues = accumulatorValues;

							for (auto resultValue : rva)
							{
								auto returnValue = new values::indexed_value { argument.index, resultValue };

								scopedReturnValues.push(returnValue);
							}

							results.push_back({ rep, scopedReturnValues });
						}
					}
				}

				return results;
			});

		for (auto& [subject_entry_point, subject_values] : subject_results)
		{
			std::multimap<int, const base_value*, std::greater<int>> indexed_values;

			for (auto& value : subject_values)
			{
				auto indexed = static_cast<const values::indexed_value*>(value);

				indexed_values.insert({ indexed->index(), indexed->subject() });
			}

			value_stack pointers_to_values;

			for (auto [_, value] : indexed_values)
			{
				pointers_to_values.push(value);
			}

			auto llvm_context = subject_entry_point.context();

			auto preliminary_block = create_block(llvm_context);

			auto preliminary_entry_point = subject_entry_point.with_block(preliminary_block);

			auto pod = new pod_fragment(call_, context.values);
			auto call_results = pod->build(visitor, { preliminary_entry_point, pointers_to_values });

			auto produces_iterator = std::any_of(begin(call_results), end(call_results), [](auto pair) {
				auto& [_, call_values] = pair;

				if (call_values.size() <= 0)
				{
					return false;
				}

				auto return_value = call_values.peek();

				return return_value->id() == value_id::tuple;
			});

			if (produces_iterator)
			{
				auto iterator = new values::
					post_lazy_value(pod, new types::iterator_type(), preliminary_entry_point, pointers_to_values, call_results);

				result.push_back({ subject_entry_point, iterator });
			}
			else
			{
				link_blocks(subject_entry_point.block(), preliminary_block);

				for (auto& [call_entry_point, call_values] : call_results)
				{
					auto consumer_results = accept(consumer_, visitor, { call_entry_point, call_values });

					for (auto& consumer_result : consumer_results)
					{
						result.push_back(consumer_result);
					}
				}
			}
		}

		return result;
	}
}
