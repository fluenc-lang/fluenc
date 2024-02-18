#include <fmt/core.h>

#include "exceptions/invalid_function_pointer_type_exception.hpp"

invalid_function_pointer_type_exception::invalid_function_pointer_type_exception(
	const std::shared_ptr<peg::Ast>& ast,
	const std::string& name
)
	: compiler_exception(ast)
	, name_(name)
{
}

std::string invalid_function_pointer_type_exception::message() const
{
	return fmt::format("'{}' is not of function pointer type", name_);
}
