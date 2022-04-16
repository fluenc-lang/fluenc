#include "exceptions/InvalidTypeException.h"

InvalidTypeException::InvalidTypeException(const TokenInfo &token, const std::string &expectedType, const std::string &actualType)
	: CompilerException(token)
	, m_expectedType(expectedType)
	, m_actualType(actualType)
{
}

std::string InvalidTypeException::message() const
{
	std::ostringstream stream;
	stream << "Invalid type. Expected ";
	stream << m_expectedType;
	stream << ", got ";
	stream << m_actualType;

	return stream.str();
}
