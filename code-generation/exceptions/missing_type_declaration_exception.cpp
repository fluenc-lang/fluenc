#include <fmt/core.h>

#include "missing_type_declaration_exception.hpp"

missing_type_declaration_exception::missing_type_declaration_exception(
	const std::shared_ptr<peg::Ast>& ast,
	const std::string& type_name,
	const std::string& field_name
)
	: compiler_exception(ast)
	, type_name_(type_name)
	, field_name_(field_name)
{
}

std::string missing_type_declaration_exception::message() const
{
	return fmt::format("Field '{}' for type '{}' lacks type specifier", field_name_, type_name_);
}
