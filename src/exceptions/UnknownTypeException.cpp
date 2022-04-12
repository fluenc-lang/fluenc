#include "exceptions/UnknownTypeException.h"

UnknownTypeException::UnknownTypeException(const TokenInfo &token, const std::string &typeName)
	: CompilerException(token)
	, m_typeName(typeName)
{
}

std::string UnknownTypeException::message() const
{
	std::stringstream stream;
	stream << "Type '" << m_typeName << "' was not found";

	return stream.str();
}
