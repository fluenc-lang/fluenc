#include "exceptions/InvalidFunctionPointerTypeException.h"

InvalidFunctionPointerTypeException::InvalidFunctionPointerTypeException(const TokenInfo &token, const std::string &name)
	: CompilerException(token)
	, m_name(name)
{
}

std::string InvalidFunctionPointerTypeException::message() const
{
	std::stringstream stream;
	stream << "'" << m_name << "' is not of function pointer type";

	return stream.str();
}
