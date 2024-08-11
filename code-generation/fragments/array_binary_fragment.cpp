#include "ast.hpp"
#include "binary_fragment.hpp"

#include "generators/aggregate_iterator_generator.hpp"

#include "values/expanded_lazy_value.hpp"

namespace fluenc::code_generation::fragments
{
	std::vector<emitter_result> array_binary_fragment::build(const emitter& visitor, emitter_context context) const
	{
		auto left = context.values.require<values::pre_lazy_value>(binary_->ast);
		auto right = context.values.require<values::pre_lazy_value>(binary_->ast);

		if (binary_->op == "|")
		{
			std::vector<const base_value*> inputs({ new values::expanded_lazy_value(right),
													new values::expanded_lazy_value(left) });

			auto generator = new generators::aggregate_iterator_generator(binary_, inputs);
			auto lazy = new values::pre_lazy_value(generator, context.entryPoint);

			context.values.push(lazy);

			return accept(binary_->consumer, visitor, { context.entryPoint, context.values });
		}

		auto operand_type = left->type();
		auto operand_type_name = operand_type->name();

		throw invalid_operator_exception(binary_->ast, binary_->op, operand_type_name);
	}
}
