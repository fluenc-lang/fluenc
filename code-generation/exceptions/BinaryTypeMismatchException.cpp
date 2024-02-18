#include <sstream>

#include <fmt/core.h>

#include "exceptions/BinaryTypeMismatchException.h"

BinaryTypeMismatchException::BinaryTypeMismatchException(const std::shared_ptr<peg::Ast> &ast
	, const std::string &leftTypeName
	, const std::string &rightTypeName
	)
	: CompilerException(ast)
	, m_leftTypeName(leftTypeName)
	, m_rightTypeName(rightTypeName)
{
}

std::string BinaryTypeMismatchException::message() const
{
	return fmt::format("Attempted binary operation on values with incompatible types ({}) and ({})", m_leftTypeName, m_rightTypeName);
}
