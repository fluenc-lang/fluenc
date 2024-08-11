#include "ir_builder.hpp"
#include "unary_fragment.hpp"
#include "utility.hpp"
#include "value_helper.hpp"

#include "values/scalar_value.hpp"

#include "exceptions/invalid_operator_exception.hpp"

namespace fluenc::code_generation::fragments
{
	std::vector<emitter_result> boolean_unary_fragment::build(const emitter& visitor, emitter_context context) const
	{
		UNUSED(visitor);

		auto operand = value_helper::get_scalar(unary_->ast, context.entryPoint, context.values);

		ir_builder builder(context.entryPoint);

		auto valueFactory = [&] {
			if (unary_->op == "!")
			{
				return builder.create_not(operand);
			}

			auto operandType = operand->type();
			auto operandTypeName = operandType->name();

			throw invalid_operator_exception(unary_->ast, unary_->op, operandTypeName);
		};

		auto value = valueFactory();

		context.values.push(value);

		return { { context.entryPoint, context.values } };
	}
}
