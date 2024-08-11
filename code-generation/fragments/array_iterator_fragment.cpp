#include "array_iterator_fragment.hpp"
#include "array_element_fragment.hpp"
#include "emitter.hpp"
#include "index_injector_fragment.hpp"
#include "index_repeater_fragment.hpp"
#include "utility.hpp"

#include "values/reference_value.hpp"

#include "iterators/index_iterator.hpp"

#include <numeric>

namespace fluenc::code_generation::fragments
{
	array_iterator_fragment::array_iterator_fragment(
		const std::shared_ptr<peg::Ast>& ast,
		const base_type* type,
		const std::vector<emitter_result>& values,
		size_t size
	)
		: ast_(ast)
		, values_(values)
		, type_(type)
		, size_(size)
	{
	}

	std::vector<emitter_result> array_iterator_fragment::build(const emitter& visitor, emitter_context context) const
	{
		auto llvm_context = context.entryPoint.context();
		auto block = context.entryPoint.block();

		std::vector<emitter_result> results;

		auto index = context.values.require<values::reference_value>(ast_);

		auto iterator = std::accumulate(
			fluenc::index_iterator(0u),
			fluenc::index_iterator(size_),
			static_cast<fragment*>(nullptr),
			[&](auto next, auto) {
				return new array_element_fragment(ast_, type_, new index_repeater_fragment(this), next);
			}
		);

		for (auto [_, element_values] : values_)
		{
			element_values.push(index);

			auto array_block = llvm::BasicBlock::Create(*llvm_context, "array");

			link_blocks(block, array_block);

			auto iterator_entry_point = context.entryPoint.with_block(array_block).with_name("__array").mark_entry();

			for (auto& result : iterator->build(visitor, { iterator_entry_point, element_values }))
			{
				results.push_back(result);
			}
		}

		return results;
	}
}
