#include <llvm/IR/Constants.h>

#include "aggregate_iterator_generator.hpp"
#include "ir_builder.hpp"
#include "utility.hpp"

#include "fragments/aggregate_iterator_fragment.hpp"

#include "types/array_type.hpp"
#include "types/int64_type.hpp"
#include "types/tuple_type.hpp"
#include "types/without_type.hpp"

#include "values/scalar_value.hpp"

namespace fluenc::code_generation::generators
{
	aggregate_iterator_generator::aggregate_iterator_generator(
		const expression_t& node,
		const std::vector<const base_value*>& input
	)
		: node_(node)
		, input_(input)
	{
	}

	std::vector<emitter_result> aggregate_iterator_generator::generate(const emitter &visitor, emitter_context context, generation_mode mode) const
	{
		UNUSED(mode);

		auto llvm_context = context.entryPoint.context();

		auto index_type = types::int64_type::instance();

		auto storage_type = index_type->storage_type(*llvm_context);

		auto zero = new values::scalar_value(index_type, llvm::ConstantInt::get(storage_type, 0));

		auto index = context.entryPoint.alloc(index_type, "index");

		ir_builder builder(context.entryPoint);

		builder.create_store(zero, index);

		return (new fragments::aggregate_iterator_fragment(index, node_, type(), input_))->build(visitor, context);
	}

	const base_generator* aggregate_iterator_generator::clone(const entry_point& entryPoint, clone_strategy strategy) const
	{
		UNUSED(entryPoint);
		UNUSED(strategy);

		return this;
	}

	const base_type* aggregate_iterator_generator::type() const
	{
		std::map<size_t, std::array<const base_type*, 2>> map;

		for (auto i = 0u; i < input_.size(); i++)
		{
			auto type = input_[i]->type();

			if (auto array = type_cast<const types::array_type*>(type))
			{
				auto types = array->types();

				for (auto j = 0u; j < types.size(); j++)
				{
					map[j][i] = types[j];
				}
			}
			else
			{
				throw std::exception();
			}
		}

		std::vector<const base_type*> element_types;

		std::transform(begin(map), end(map), back_inserter(element_types), [](auto pair) {
			auto [_, input] = pair;

			std::vector<const base_type*> types;

			std::transform(begin(input), end(input), back_inserter(types), [](auto type) -> const base_type* {
				if (!type)
				{
					return types::without_type::instance();
				}

				return type;
			});

			return types::tuple_type::get(types);
		});

		return types::array_type::get(element_types);
	}
}
