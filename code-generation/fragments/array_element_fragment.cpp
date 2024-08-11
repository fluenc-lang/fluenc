#include "array_element_fragment.hpp"
#include "emitter.hpp"
#include "ir_builder.hpp"
#include "utility.hpp"

#include "fragments/array_continuation_fragment.hpp"

#include "values/expandable_value.hpp"
#include "values/indexed_value.hpp"
#include "values/reference_value.hpp"
#include "values/scalar_value.hpp"
#include "values/tuple_value.hpp"

#include "types/iterator_type.hpp"

#include <llvm/IR/Constants.h>

namespace fluenc::code_generation::fragments
{
	array_element_fragment::array_element_fragment(
		const std::shared_ptr<peg::Ast>& ast,
		const base_type* array_type,
		const fragment* node,
		const fragment* next
	)
		: ast_(ast)
		, array_type_(array_type)
		, node_(node)
		, next_(next)
	{
	}

	std::vector<emitter_result> array_element_fragment::build(const emitter& visitor, emitter_context context) const
	{
		value_stack values_if_true;

		auto llvmContext = context.entryPoint.context();
		auto module = context.entryPoint.module();

		auto dataLayout = module->getDataLayout();

		auto index = context.values.require<values::reference_value>(ast_);
		auto value = context.values.require<values::indexed_value>(ast_);

		if (next_)
		{
			auto values_if_false = context.values;

			auto index_type = index->type();
			auto storage_type = index_type->storage_type(*llvmContext);

			auto ifTrue = create_block(llvmContext);
			auto ifFalse = create_block(llvmContext);

			ir_builder builder(context.entryPoint);

			auto index_load = builder.create_load(index, "index");

			auto index_constant = new values::scalar_value(index_type, llvm::ConstantInt::get(storage_type, value->index()));

			auto comparison = builder.create_cmp(llvm::CmpInst::Predicate::ICMP_EQ, index_load, index_constant);

			builder.create_cond_br(comparison, ifTrue, ifFalse);

			auto ep_if_false = context.entryPoint.with_index(value->index()).with_block(ifFalse);

			auto ep_if_true = context.entryPoint.with_index(value->index()).with_block(ifTrue);

			values_if_false.push(index);

			auto continuation = new fragments::array_continuation_fragment(index, node_, types::iterator_type::instance());
			auto expandable = new values::expandable_value(true, array_type_, context.entryPoint, continuation, index);

			auto tuple = new values::tuple_value({ expandable, value->subject() });

			values_if_true.push(tuple);

			auto results_if_false = next_->build(visitor, { ep_if_false, values_if_false });

			std::vector<emitter_result> result = { { ep_if_true, values_if_true } };

			result.insert(end(result), begin(results_if_false), end(results_if_false));

			return result;
		}

		auto ep = context.entryPoint.with_index(value->index());

		context.values.push(value->subject());

		return { { ep, context.values } };
	}
}
