#include <fmt/core.h>

#include "missing_default_value_exception.hpp"

missing_default_value_exception::missing_default_value_exception(
	const std::shared_ptr<peg::Ast>& ast,
	const std::string& fieldName
)
	: compiler_exception(ast)
	, field_name_(fieldName)
{
}

std::string missing_default_value_exception::message() const
{
	return fmt::format("Field '{}' has no default value and needs to be initialized", field_name_);
}
