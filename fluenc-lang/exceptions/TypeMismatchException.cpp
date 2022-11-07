#include <sstream>

#include "exceptions/TypeMismatchException.h"

TypeMismatchException::TypeMismatchException(const std::shared_ptr<peg::Ast> &ast
	, const std::string &leftTypeName
	, const std::string &rightTypeName
	)
	: CompilerException(ast)
	, m_leftTypeName(leftTypeName)
	, m_rightTypeName(rightTypeName)
{
}

std::string TypeMismatchException::message() const
{
	std::ostringstream stream;
	stream << "Attempted binary operation on values with incompatible types (";
	stream << m_leftTypeName;
	stream << " and ";
	stream << m_rightTypeName;
	stream << ")";

	return stream.str();
}
