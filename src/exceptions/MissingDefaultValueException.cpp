#include "exceptions/MissingDefaultValueException.h"

MissingDefaultValueException::MissingDefaultValueException(const TokenInfo &token, const std::string &fieldName)
	: CompilerException(token)
	, m_fieldName(fieldName)
{
}

std::string MissingDefaultValueException::message() const
{
	std::ostringstream stream;
	stream << "Field '";
	stream << m_fieldName;
	stream << "' has no default value and needs to be initialized";

	return stream.str();
}
