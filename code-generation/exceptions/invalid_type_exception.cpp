#include <fmt/core.h>

#include "invalid_type_exception.hpp"

invalid_type_exception::invalid_type_exception(
	const std::shared_ptr<peg::Ast>& ast,
	const std::string& expected_type,
	const std::string& actual_type
)
	: compiler_exception(ast)
	, expected_type_(expected_type)
	, actual_type_(actual_type)
{
}

std::string invalid_type_exception::message() const
{
	return fmt::format("Invalid type. Expected '{}', got '{}'", expected_type_, actual_type_);
}
