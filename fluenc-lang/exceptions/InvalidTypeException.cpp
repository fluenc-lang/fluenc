#include <sstream>

#include <fmt/core.h>

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
	return fmt::format("Invalid type. Expected '{}', got '{}'", m_expectedType, m_actualType);
}
