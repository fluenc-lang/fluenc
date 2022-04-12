#include "exceptions/MissingTypeDeclarationException.h"

MissingTypeDeclarationException::MissingTypeDeclarationException(const TokenInfo &token
	, const std::string &typeName
	, const std::string &fieldName
	)
	: CompilerException(token)
	, m_typeName(typeName)
	, m_fieldName(fieldName)
{
}

std::string MissingTypeDeclarationException::message() const
{
	std::stringstream stream;
	stream << "Field '" << m_fieldName;
	stream << "' for type '" << m_typeName;
	stream << "' lacks type specifier";

	return stream.str();
}
