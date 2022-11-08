#include <sstream>

#include <fmt/core.h>

#include "exceptions/InvalidOperatorException.h"

InvalidOperatorException::InvalidOperatorException(const std::shared_ptr<peg::Ast> &ast, const std::string &op, const std::string &typeName)
	: CompilerException(ast)
	, m_op(op)
	, m_typeName(typeName)
{
}

std::string InvalidOperatorException::message() const
{
	return fmt::format("Operator '{}' is not defined for type '{}'", m_op, m_typeName);
}
