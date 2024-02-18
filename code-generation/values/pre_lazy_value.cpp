#include <unordered_map>

#include "base_generator.hpp"
#include "emitter.hpp"
#include "fragment.hpp"
#include "function_helper.hpp"
#include "type_compatibility_calculator.hpp"
#include "utility.hpp"
#include "value_helper.hpp"

#include "types/array_type.hpp"

#include "values/expandable_value.hpp"
#include "values/pre_lazy_value.hpp"
#include "values/tuple_value.hpp"

#include "fragments/post_evaluation_fragment.hpp"

using element_type = std::pair<bool, const fluenc::code_generation::base_type*>;

namespace fluenc::code_generation::values
{
	pre_lazy_value::pre_lazy_value(const generators::base_generator* generator, const entry_point& entryPoint)
		: generator_(generator)
		, entry_point_(new entry_point(entryPoint))
		, type_(nullptr)
	{
	}

	value_id pre_lazy_value::id() const
	{
		return value_id::pre_lazy;
	}

	element_type get_element_type(element_type seed, const entry_point& entryPoint, value_stack values)
	{
		return accumulate(values.begin(), values.end(), seed, [&](auto accumulated, auto value) -> element_type {
			if (auto tuple = value_cast<const tuple_value*>(value))
			{
				return get_element_type({ false, accumulated.second }, entryPoint, tuple->values());
			}

			if (auto expandable = value_cast<const expandable_value*>(value))
			{
				auto chain = expandable->chain();
				auto provider = expandable->provider();

				auto continuation_entry_point = provider->with_block(entryPoint.block());

				emitter analyzer({});

				auto chain_result = chain->build(analyzer, { continuation_entry_point, value_stack() });

				auto [_, chain_values] = *chain_result.begin();

				return get_element_type(accumulated, *provider, chain_values);
			}

			if (auto expanded = value_cast<const expanded_value*>(value))
			{
				return { expanded->is_array(), expanded->type() };
			}

			return { accumulated.first, value->type() };
		});
	}

	std::vector<emitter_result> expand_iterator(const emitter& emitter, emitter_context context)
	{
		fragments::post_evaluation_fragment node;

		return node.build(emitter, context);
	}

	const base_type* pre_lazy_value::type() const
	{
		if (type_)
		{
			return type_;
		}

		auto getType = [&]() -> const base_type* {
			auto context = entry_point_->context();

			auto block = create_block(context);
			auto alloc = create_block(context);

			link_blocks(alloc, block);

			auto entryPoint = (*entry_point_).with_block(block).with_alloc(alloc);

			emitter analyzer({});

			std::vector<emitter_result> results;

			for (auto& [result_entry_point, result_values] :
				 generator_->generate(analyzer, { entryPoint, value_stack() }, generators::generation_mode::dry_run))
			{
				auto result_block = create_block(context);

				auto with_block = result_entry_point.with_block(result_block);

				for (auto& result : expand_iterator(analyzer, { with_block, result_values }))
				{
					results.push_back(result);
				}
			}

			std::map<int, const base_type*> types_by_index;

			for (auto& [result_entry_point, result_values] : results)
			{
				auto result_block = create_block(context);

				auto with_block = result_entry_point.with_block(result_block);

				auto [isArrayCompatible, type] = get_element_type({ true, nullptr }, with_block, result_values);

				if (!isArrayCompatible)
				{
					return generator_->type();
				}

				auto index = result_entry_point.index();

				if (auto existing = types_by_index.find(index); existing != end(types_by_index))
				{
					if (type_compatibility_calculator::calculate(entryPoint, existing->second, type) != 0)
					{
						return generator_->type();
					}
				}

				types_by_index[index] = type;
			}

			std::vector<const base_type*> types;

			transform(begin(types_by_index), end(types_by_index), back_inserter(types), [](auto pair) {
				return pair.second;
			});

			return types::array_type::get(types);
		};

		return type_ = getType();
	}

	const base_value* pre_lazy_value::clone(const entry_point& entry_point, clone_strategy strategy) const
	{
		auto cloned = generator_->clone(entry_point, strategy);

		return new pre_lazy_value(cloned, entry_point);
	}

	std::vector<emitter_result> pre_lazy_value::generate(const emitter& visitor, emitter_context context) const
	{
		return generator_->generate(visitor, context, generators::generation_mode::regular);
	}

	entry_point pre_lazy_value::assign_from(const entry_point& entryPoint, const base_value* source, const emitter& emitter) const
	{
		auto context = entryPoint.context();

		std::vector<emitter_result> target_results;

		for (auto& [result_entry_point, result_values] :
			 generator_->generate(emitter, { entryPoint, value_stack() }, generators::generation_mode::regular))
		{
			for (auto& result : expand_iterator(emitter, { result_entry_point, result_values }))
			{
				target_results.push_back(result);
			}
		}

		std::unordered_map<uint32_t, const base_value*> indexed_results;

		auto target_exit_block = llvm::BasicBlock::Create(*context, "targetExit");

		for (auto [result_entry_point, resultValues] : target_results)
		{
			auto values = value_helper::extract_values<base_value>(resultValues);

			if (values.size() <= 0)
			{
				throw std::exception();
			}

			indexed_results.insert({ result_entry_point.index(), values[0] });

			link_blocks(result_entry_point.block(), target_exit_block);
		}

		auto source_entry_point = entryPoint.with_block(target_exit_block);

		auto source_exit_block = llvm::BasicBlock::Create(*context, "sourceExit");

		std::vector<emitter_result> source_results = { { source_entry_point, source } };

		for (auto i = 0u; i < source_results.size(); i++)
		{
			auto [source_result_entry_point, source_result_values] = source_results[i];

			auto source_value = source_result_values.pop();
			auto target_value = indexed_results[source_result_entry_point.index()];

			auto source_tuple_value = value_cast<const tuple_value*>(source_value);

			if (source_tuple_value)
			{
				auto source_tuple_values = source_tuple_value->values();

				auto result_entry_point = value_helper::
					transfer_value(source_result_entry_point, source_tuple_values.pop(), target_value, emitter);

				auto source_continuation = source_tuple_values.require<expandable_value>(nullptr);

				auto source_chain = source_continuation->chain();
				auto source_provider = source_continuation->provider();

				auto source_continuation_entry_point = source_provider->with_block(result_entry_point.block());

				for (auto& [chain_entry_point, chain_values] :
					 source_chain->build(emitter, { source_continuation_entry_point, value_stack() }))
				{
					auto source_value = chain_values.require<expanded_value>(nullptr);

					auto loop_target = function_helper::find_tail_call_target(&chain_entry_point, value_stack(source_value));
					auto result_entry_point = value_helper::
						transfer_value(chain_entry_point, source_value, source_continuation, emitter);

					auto loop_target_entry = loop_target->entry();

					link_blocks(result_entry_point.block(), loop_target_entry->block());
				}
			}
			else
			{
				auto result_entry_point = value_helper::
					transfer_value(source_result_entry_point, source_value, target_value, emitter);

				link_blocks(result_entry_point.block(), source_exit_block);
			}
		}

		return entryPoint.with_block(source_exit_block);
	}

	entry_point pre_lazy_value::assign_from(const entry_point& entryPoint, const pre_lazy_value* source, const emitter& emitter) const
	{
		auto context = entryPoint.context();

		std::vector<emitter_result> target_results;

		for (auto& [result_entry_point, result_values] :
			 generator_->generate(emitter, { entryPoint, value_stack() }, generators::generation_mode::regular))
		{
			for (auto& result : expand_iterator(emitter, { result_entry_point, result_values }))
			{
				target_results.push_back(result);
			}
		}

		std::unordered_map<uint32_t, const base_value*> indexed_results;

		auto target_exit_block = llvm::BasicBlock::Create(*context, "targetExit");

		for (auto [result_entry_point, result_values] : target_results)
		{
			auto values = value_helper::extract_values<base_value>(result_values);

			if (values.size() <= 0)
			{
				throw std::exception();
			}

			indexed_results.insert({ result_entry_point.index(), values[0] });

			link_blocks(result_entry_point.block(), target_exit_block);
		}

		auto source_entry_point = entryPoint.with_block(target_exit_block);

		auto source_exit_block = llvm::BasicBlock::Create(*context, "sourceExit");

		std::vector<emitter_result> source_results;

		for (auto& [result_entry_point, result_values] :
			 source->generate(emitter, { source_entry_point, value_stack() }))
		{
			for (auto& result : expand_iterator(emitter, { result_entry_point, result_values }))
			{
				source_results.push_back(result);
			}
		}

		for (auto i = 0u; i < source_results.size(); i++)
		{
			auto [source_result_entry_point, source_result_values] = source_results[i];

			auto source_value = source_result_values.pop();
			auto target_value = indexed_results[source_result_entry_point.index()];

			auto source_tuple_value = value_cast<const tuple_value*>(source_value);

			if (source_tuple_value)
			{
				auto source_tuple_values = source_tuple_value->values();

				auto result_entry_point = value_helper::
					transfer_value(source_result_entry_point, source_tuple_values.pop(), target_value, emitter);

				auto source_continuation = source_tuple_values.require<expandable_value>(nullptr);

				auto source_cain = source_continuation->chain();
				auto source_provider = source_continuation->provider();

				auto source_continuation_entry_point = source_provider->with_block(result_entry_point.block());

				for (auto& [chain_entry_point, chain_values] :
					 source_cain->build(emitter, { source_continuation_entry_point, value_stack() }))
				{
					auto source_value = chain_values.require<expanded_value>(nullptr);

					auto loop_target = function_helper::find_tail_call_target(&chain_entry_point, value_stack(source_value));
					auto result_entry_point = value_helper::
						transfer_value(chain_entry_point, source_value, source_continuation, emitter);

					auto loop_target_entry = loop_target->entry();

					link_blocks(result_entry_point.block(), loop_target_entry->block());
				}
			}
			else
			{
				auto result_entry_point = value_helper::
					transfer_value(source_result_entry_point, source_value, target_value, emitter);

				link_blocks(result_entry_point.block(), source_exit_block);
			}
		}

		return entryPoint.with_block(source_exit_block);
	}
}
