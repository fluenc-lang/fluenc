#include <fmt/core.h>

#include "missing_field_exception.hpp"

missing_field_exception::missing_field_exception(
	const std::shared_ptr<peg::Ast>& ast,
	const std::string& typeName,
	const std::string& fieldName
)
	: compiler_exception(ast)
	, type_name_(typeName)
	, field_name_(fieldName)
{
}

std::string missing_field_exception::message() const
{
	return fmt::format("Field '{}' in type '{}' does not exist", field_name_, type_name_);
}
