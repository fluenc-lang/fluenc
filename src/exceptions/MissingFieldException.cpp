#include "include/exceptions/MissingFieldException.h"

MissingFieldException::MissingFieldException(const TokenInfo &token, const std::string &typeName, const std::string &fieldName)
	: CompilerException(token)
	, m_typeName(typeName)
	, m_fieldName(fieldName)
{
}

std::string MissingFieldException::message() const
{
	std::ostringstream stream;
	stream << "Field '";
	stream << m_fieldName;
	stream << "' in type '";
	stream << m_typeName;
	stream << "' does not exist";

	return stream.str();
}
