#include "InvalidArgumentTypeException.h"

InvalidArgumentTypeException::InvalidArgumentTypeException(ParserRuleContext *context)
	: CompilerException(context)
{
}

std::string InvalidArgumentTypeException::message() const
{
	return "Arguments of tuple type can not be used for imported functions";
}
