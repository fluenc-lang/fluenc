#include "ast.hpp"
#include "unary_fragment.hpp"

#include "values/forwarded_value.hpp"

#include "exceptions/invalid_operator_exception.hpp"

namespace fluenc::code_generation::fragments
{
	std::vector<emitter_result> string_unary_fragment::build(const emitter& visitor, emitter_context context) const
	{
		auto operand = context.values.pop();

		auto value_factory = [&] {
			if (unary_->op == "@")
			{
				return new values::forwarded_value(operand);
			}

			auto operand_type = operand->type();
			auto operand_type_name = operand_type->name();

			throw invalid_operator_exception(unary_->ast, unary_->op, operand_type_name);
		};

		auto value = value_factory();

		context.values.push(value);

		return accept(unary_->consumer, visitor, context);
	}
}
