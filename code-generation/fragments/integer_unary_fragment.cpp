#include "unary_fragment.hpp"
#include "utility.hpp"

#include "exceptions/invalid_operator_exception.hpp"

namespace fluenc::code_generation::fragments
{
	std::vector<emitter_result> integer_unary_fragment::build(const emitter& visitor, emitter_context context) const
	{
		UNUSED(visitor);

		auto operand = context.values.pop();

		auto operand_type = operand->type();
		auto operand_type_name = operand_type->name();

		throw invalid_operator_exception(unary_->ast, unary_->op, operand_type_name);
	}
}
