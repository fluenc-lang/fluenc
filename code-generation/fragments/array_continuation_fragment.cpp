#include "array_continuation_fragment.hpp"
#include "emitter.hpp"
#include "ir_builder.hpp"
#include "utility.hpp"

#include "values/expanded_value.hpp"
#include "values/scalar_value.hpp"

#include "types/int64_type.hpp"

#include <llvm/IR/Constants.h>

namespace fluenc::code_generation::fragments
{
	array_continuation_fragment::array_continuation_fragment(
		const values::reference_value* index,
		const fragment* node,
		const base_type* iterator_type
	)
		: index_(index)
		, node_(node)
		, iterator_type_(iterator_type)
	{
	}

	std::vector<emitter_result> array_continuation_fragment::build(const emitter& visitor, emitter_context context) const
	{
		UNUSED(visitor);

		auto llvm_context = context.entryPoint.context();

		auto index_type = types::int64_type::instance();
		auto storage_type = index_type->storage_type(*llvm_context);

		auto index_constant = new values::scalar_value(index_type, llvm::ConstantInt::get(storage_type, 1));

		ir_builder builder(context.entryPoint);

		auto load = builder.create_load(index_, "index");
		auto add = builder.create_add(load, index_constant);

		auto value = new values::expanded_value(true, iterator_type_, context.entryPoint, node_, {}, add);

		return { { context.entryPoint, value } };
	}
}
