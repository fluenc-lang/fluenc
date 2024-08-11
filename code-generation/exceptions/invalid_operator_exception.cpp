#include <fmt/core.h>

#include "invalid_operator_exception.hpp"

invalid_operator_exception::invalid_operator_exception(
	const std::shared_ptr<peg::Ast>& ast,
	const std::string& op,
	const std::string& type_name
)
	: compiler_exception(ast)
	, operator_(op)
	, type_name_(type_name)
{
}

std::string invalid_operator_exception::message() const
{
	return fmt::format("Operator '{}' is not defined for type '{}'", operator_, type_name_);
}
