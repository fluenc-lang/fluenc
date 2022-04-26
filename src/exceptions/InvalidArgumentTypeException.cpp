#include <string>

#include "exceptions/InvalidArgumentTypeException.h"

InvalidArgumentTypeException::InvalidArgumentTypeException(const std::shared_ptr<peg::Ast> &ast)
	: CompilerException(ast)
{
}

std::string InvalidArgumentTypeException::message() const
{
	return "Arguments of tuple type can not be used for imported functions";
}
