#include "exceptions/InvalidArgumentTypeException.h"

InvalidArgumentTypeException::InvalidArgumentTypeException(const TokenInfo &token)
	: CompilerException(token)
{
}

std::string InvalidArgumentTypeException::message() const
{
	return "Arguments of tuple type can not be used for imported functions";
}
