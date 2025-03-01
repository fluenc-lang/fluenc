#include <numeric>

#include <llvm/IR/Constants.h>

#include "aggregate_iterator_element_fragment.hpp"
#include "aggregate_iterator_fragment.hpp"
#include "array_continuation_fragment.hpp"
#include "continuation_fragment.hpp"
#include "emitter.hpp"
#include "expression_adapter_fragment.hpp"
#include "ir_builder.hpp"
#include "post_evaluation_fragment.hpp"
#include "pre_evaluation_fragment.hpp"
#include "terminator_fragment.hpp"
#include "utility.hpp"
#include "value_helper.hpp"

#include "iterators/index_iterator.hpp"

#include "types/expanded_type.hpp"
#include "types/int64_type.hpp"
#include "types/iterator_type.hpp"

#include "values/expandable_value.hpp"
#include "values/expanded_value.hpp"
#include "values/placeholder_value.hpp"
#include "values/reference_value.hpp"
#include "values/scalar_value.hpp"
#include "values/tuple_value.hpp"
#include "values/without_value.hpp"

namespace fluenc::code_generation::fragments
{
	aggregate_iterator_fragment::aggregate_iterator_fragment(
		const values::reference_value* index,
		const expression_t& node,
		const base_type* array_type,
		const std::vector<const base_value*>& iteratables
	)
		: index_(index)
		, node_(node)
		, array_type_(array_type)
		, values_(iteratables)
	{
	}

	std::vector<emitter_result> aggregate_iterator_fragment::build(const emitter& visitor, emitter_context context) const
	{
		using std::begin, std::end, std::size;

		using values_t = std::vector<std::pair<size_t, value_stack_t>>;
		using results_t = std::map<size_t, std::array<const base_value*, 2>>;

		auto llvm_context = context.entryPoint.context();

		auto pre_evaluation = new pre_evaluation_fragment(terminator_fragment::instance());
		auto post_evaluation = new post_evaluation_fragment();

		values_t values_by_index;

		auto pre_entry_point = std::
			accumulate(index_iterator(), index_iterator(size(values_)), context.entryPoint, [&](auto entryPoint, auto index) {
				for (auto& [result_entry_point, result_values] :
					 pre_evaluation->build(visitor, { entryPoint, values_[index] }))
				{
					values_by_index.push_back({ index, result_values });

					return result_entry_point;
				}

				return entryPoint;
			});

		results_t results_by_index;

		auto post_entry_point = std::
			accumulate(begin(values_by_index), end(values_by_index), pre_entry_point, [&](auto entryPoint, auto pair) {
				auto block = llvm::BasicBlock::Create(*llvm_context, "merge");

				auto evaluationResults = post_evaluation->build(visitor, { entryPoint, pair.second });

				return std::accumulate(begin(evaluationResults), end(evaluationResults), entryPoint, [&](auto, auto result) {
					auto& storage = results_by_index[result.entryPoint.index()];

					storage[pair.first] = result.values.pop();

					link_blocks(result.entryPoint.block(), block);

					return result.entryPoint.with_block(block);
				});
			});

		std::vector<emitter_result> results;

		auto array_continuation = new array_continuation_fragment(index_, this, types::iterator_type::instance());

		auto last_entry_point = std::
			accumulate(begin(results_by_index), prev(end(results_by_index)), post_entry_point, [&](auto entry_point, auto pair) {
				auto [i, values] = pair;

				auto array_expandable = new values::
					expandable_value(true, array_type_, entry_point, array_continuation, index_);

				std::vector<const base_value*> elements;
				std::vector<const base_value*> dependencies = {
					new values::tuple_value({ array_expandable, values::placeholder_value::instance() })
				};

				auto init = new continuation_fragment(
					new aggregate_iterator_element_fragment(array_expandable, terminator_fragment::instance()),
					new expression_adapter_fragment(node_)
				);

				auto chain = std::accumulate(
					begin(values),
					end(values),
					static_cast<const fragment*>(init),
					[&](auto next, auto value) -> const fragment* {
						if (!value)
						{
							elements.push_back(values::without_value::instance());
						}
						else if (auto tuple = value_cast<const values::tuple_value*>(value))
						{
							dependencies.insert(begin(dependencies), tuple);

							auto tuple_elements = tuple->values();

							auto actual_value = tuple_elements.pop();

							elements.push_back(actual_value);

							auto expandable = require<values::expandable_value>(tuple_elements, nullptr);

							return new aggregate_iterator_element_fragment(expandable, next);
						}
						else
						{
							elements.push_back(value);
						}

						return next;
					}
				);

				ir_builder builder(entry_point);

				auto index_load = builder.create_load(index_, "index");

				auto index_type = types::int64_type::instance();

				auto storage_type = index_type->storage_type(*llvm_context);

				auto index_constant = new values::scalar_value(index_type, llvm::ConstantInt::get(storage_type, i));

				auto if_true = create_block(llvm_context);
				auto if_false = create_block(llvm_context);

				auto comparison = builder.create_cmp(llvm::CmpInst::Predicate::ICMP_EQ, index_load, index_constant);

				builder.create_cond_br(comparison, if_true, if_false);

				auto ep_if_false = entry_point.with_index(i).with_block(if_false);

				auto ep_if_true = entry_point.with_index(i).with_block(if_true);

				std::vector<const base_type*> dependency_types = { array_type_ };

				std::transform(begin(dependencies), end(dependencies), back_inserter(dependency_types), [](auto value) {
					return value->type();
				});

				auto expandable_type = types::expanded_type::get(dependency_types);

				auto expandable = new values::expandable_value(true, expandable_type, post_entry_point, chain, dependencies);

				auto tuple_value = new values::tuple_value(elements);
				auto final_value = new values::tuple_value({ expandable, tuple_value });

				results.push_back({ ep_if_true, final_value });

				return ep_if_false;
			});

		std::vector<const base_value*> elements;

		auto [i, values] = *rbegin(results_by_index);

		std::transform(begin(values), end(values), back_inserter(elements), [](auto value) -> const base_value* {
			if (!value)
			{
				return values::without_value::instance();
			}

			return value;
		});

		results.push_back({ last_entry_point.with_index(i), new values::tuple_value(elements) });

		return results;
	}
}
