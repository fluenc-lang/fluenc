#include <fmt/core.h>

#include "exceptions/binary_type_mismatch_exception.hpp"

binary_type_mismatch_exception::binary_type_mismatch_exception(
	const std::shared_ptr<peg::Ast>& ast,
	const std::string& left_type_name,
	const std::string& right_type_name
)
	: compiler_exception(ast)
	, left_type_name_(left_type_name)
	, right_type_name_(right_type_name)
{
}

std::string binary_type_mismatch_exception::message() const
{
	return fmt::
		format("Attempted binary operation on values with incompatible types ({}) and ({})", left_type_name_, right_type_name_);
}
