#include "UndeclaredIdentifierException.h"

UndeclaredIdentifierException::UndeclaredIdentifierException(const TokenInfo &token, const std::string &identifier)
	: CompilerException(token)
	, m_identifier(identifier)
{
}

std::string UndeclaredIdentifierException::message() const
{
	std::stringstream stream;
	stream << "'" << m_identifier << "' was not declared in this scope";

	return stream.str();
}
