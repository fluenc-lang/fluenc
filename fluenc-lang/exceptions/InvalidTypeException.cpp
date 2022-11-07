#include <sstream>

#include "InvalidTypeException.h"

InvalidTypeException::InvalidTypeException(const std::shared_ptr<peg::Ast> &ast
	, const std::string &expectedType
	, const std::string &actualType
	)
	: CompilerException(ast)
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
