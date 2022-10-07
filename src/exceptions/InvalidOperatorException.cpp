#include <sstream>

#include "exceptions/InvalidOperatorException.h"

InvalidOperatorException::InvalidOperatorException(const std::shared_ptr<peg::Ast> &ast, const std::string &op, const std::string &typeName)
	: CompilerException(ast)
	, m_op(op)
	, m_typeName(typeName)
{
}

std::string InvalidOperatorException::message() const
{
	std::ostringstream stream;
	stream << "Operator ";
	stream << m_op;
	stream << " is not defined for type ";
	stream << m_typeName;

	return stream.str();
}
