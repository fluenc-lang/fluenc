#include "UndeclaredIdentifierException.h"

UndeclaredIdentifierException::UndeclaredIdentifierException(ParserRuleContext *context, const std::string &identifier)
	: CompilerException(context)
	, m_identifier(identifier)
{
}

std::string UndeclaredIdentifierException::message() const
{
	std::stringstream stream;
	stream << "'" << m_identifier << "' was not declared in this scope";

	return  stream.str();
}
