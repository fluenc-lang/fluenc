#include <fmt/core.h>

#include "unknown_type_exception.hpp"

unknown_type_exception::unknown_type_exception(const std::shared_ptr<peg::Ast>& ast, const std::string& typeName)
	: compiler_exception(ast)
	, type_name_(typeName)
{
}

std::string unknown_type_exception::message() const
{
	return fmt::format("Type '{}' was not found", type_name_);
}
