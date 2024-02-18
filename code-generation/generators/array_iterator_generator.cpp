#include <llvm/IR/Constants.h>

#include "array_iterator_generator.hpp"
#include "ir_builder.hpp"
#include "utility.hpp"

#include "values/post_lazy_value.hpp"
#include "values/scalar_value.hpp"

#include "fragments/array_iterator_fragment.hpp"
#include "fragments/index_injector_fragment.hpp"

#include "types/array_type.hpp"
#include "types/int64_type.hpp"
#include "types/iterator_type.hpp"

namespace fluenc::code_generation::generators
{
	array_iterator_generator::array_iterator_generator(
		const std::vector<emitter_result>& values,
		const std::shared_ptr<peg::Ast>& ast,
		size_t size
	)
		: values_(values)
		, ast_(ast)
		, size_(size)
	{
	}

	std::vector<emitter_result> array_iterator_generator::generate(
		const emitter& visitor,
		emitter_context context,
		generation_mode mode
	) const
	{
		UNUSED(mode);

		ir_builder builder(context.entryPoint);

		auto llvm_context = context.entryPoint.context();

		auto index_type = types::int64_type::instance();

		auto storage_type = index_type->storage_type(*llvm_context);

		auto zero = new values::scalar_value(index_type, llvm::ConstantInt::get(storage_type, 0));

		auto index = context.entryPoint.alloc(index_type, "index");

		builder.create_store(zero, index);

		auto subject = new fragments::
			index_injector_fragment(index, new fragments::array_iterator_fragment(ast_, type(), values_, size_));

		auto block = create_block(llvm_context);

		auto iterator_entry_point = context.entryPoint.with_block(block);

		auto results = subject->build(visitor, { iterator_entry_point, context.values });

		auto iterator = new values::post_lazy_value(subject, type(), iterator_entry_point, value_stack(), results);

		context.values.push(iterator);

		return { { context.entryPoint.increase_depth(), context.values } };
	}

	const base_type* array_iterator_generator::type() const
	{
		using std::begin, std::rbegin, std::rend;

		if (values_.size() == 1)
		{
			auto [_, values] = *begin(values_);

			std::vector<const base_type*> types;

			std::transform(rbegin(values), rend(values), back_inserter(types), [](auto value) {
				return value->type();
			});

			return types::array_type::get(types);
		}

		return types::iterator_type::instance();
	}

	const base_generator* array_iterator_generator::clone(const entry_point& entryPoint, clone_strategy strategy) const
	{
		using std::begin, std::end;

		if (values_.size() == 1)
		{
			auto [input_entry_point, input_values] = *begin(values_);

			std::vector<const base_value*> cloned_values;

			std::transform(begin(input_values), end(input_values), back_inserter(cloned_values), [&](auto value) {
				return value->clone(entryPoint, strategy);
			});

			return new array_iterator_generator({ { input_entry_point, cloned_values } }, ast_, size_);
		}

		throw std::exception();
	}
}
