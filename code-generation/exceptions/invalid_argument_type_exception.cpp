#include <string>

#include "invalid_argument_type_exception.hpp"

invalid_argument_type_exception::invalid_argument_type_exception(const std::shared_ptr<peg::Ast>& ast)
	: compiler_exception(ast)
{
}

std::string invalid_argument_type_exception::message() const
{
	return "Arguments of tuple type can not be used for imported functions";
}
