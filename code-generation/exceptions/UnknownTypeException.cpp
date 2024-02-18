#include <sstream>

#include "exceptions/UnknownTypeException.h"

UnknownTypeException::UnknownTypeException(const std::shared_ptr<peg::Ast> &ast, const std::string &typeName)
	: CompilerException(ast)
	, m_typeName(typeName)
{
}

std::string UnknownTypeException::message() const
{
	std::stringstream stream;
	stream << "Type '" << m_typeName << "' was not found";

	return stream.str();
}
