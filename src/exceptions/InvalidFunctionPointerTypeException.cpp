#include "InvalidFunctionPointerTypeException.h"

InvalidFunctionPointerTypeException::InvalidFunctionPointerTypeException(ParserRuleContext *context, const std::string &name)
	: CompilerException(context)
	, m_name(name)
{
}

std::string InvalidFunctionPointerTypeException::message() const
{
	std::stringstream stream;
	stream << "'" << m_name << "' is not of function pointer type";

	return stream.str();
}
