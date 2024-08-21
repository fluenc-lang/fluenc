#include "string_iterator_generator.hpp"
#include "ir_builder.hpp"
#include "utility.hpp"

#include "values/post_lazy_value.hpp"
#include "values/scalar_value.hpp"

#include "fragments/string_iterator_fragment.hpp"

#include "types/int64_type.hpp"
#include "types/iterator_type.hpp"

#include <llvm/IR/Constants.h>

namespace fluenc::code_generation::generators
{
	string_iterator_generator::string_iterator_generator(
		const values::reference_value* address,
		const values::reference_value* length
	)
		: address_(address)
		, length_(length)
	{
	}

	std::vector<emitter_result> string_iterator_generator::generate(const emitter &visitor, emitter_context context, generation_mode mode) const
	{
		UNUSED(mode);

		ir_builder builder(context.entryPoint);

		auto llvm_context = context.entryPoint.context();

		auto index_type = types::int64_type::instance();
		auto storage_type = index_type->storage_type(*llvm_context);

		auto zero = new values::scalar_value(index_type, llvm::ConstantInt::get(storage_type, 0));

		auto index = context.entryPoint.alloc(index_type, "index");

		builder.create_store(zero, index);

		auto subject = new fragments::string_iterator_fragment(index, address_, length_);

		auto block = create_block(llvm_context);

		auto iterator_entry_point = context.entryPoint.with_block(block);

		auto results = subject->build(visitor, { iterator_entry_point, context.values });

		auto iterator = new values::post_lazy_value(subject, type(), iterator_entry_point, value_stack_t(), results);

		context.values.push(iterator);

		return { { context.entryPoint.increase_depth(), context.values } };
	}

	const base_generator* string_iterator_generator::clone(const entry_point& entryPoint, clone_strategy strategy) const
	{
		UNUSED(entryPoint);
		UNUSED(strategy);

		return this;
	}

	const base_type* string_iterator_generator::type() const
	{
		return types::iterator_type::instance();
	}
}
