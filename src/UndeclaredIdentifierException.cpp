#include <sstream>

#include "UndeclaredIdentifierException.h"

UndeclaredIdentifierException::UndeclaredIdentifierException(const std::shared_ptr<peg::Ast> &ast, const std::string &identifier)
	: CompilerException(ast)
	, m_identifier(identifier)
{
}

std::string UndeclaredIdentifierException::message() const
{
	std::stringstream stream;
	stream << "'" << m_identifier << "' was not declared in this scope";

	return stream.str();
}
