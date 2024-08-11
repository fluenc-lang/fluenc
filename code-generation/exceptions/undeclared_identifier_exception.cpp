#include <fmt/core.h>

#include "undeclared_identifier_exception.hpp"

undeclared_identifier_exception::undeclared_identifier_exception(
	const std::shared_ptr<peg::Ast>& ast,
	const std::string& identifier
)
	: compiler_exception(ast)
	, identifier_(identifier)
{
}

std::string undeclared_identifier_exception::message() const
{
	return fmt::format("'{}' was not declared in this scope", identifier_);
}
