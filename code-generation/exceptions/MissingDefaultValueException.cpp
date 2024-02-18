#include <sstream>

#include <fmt/core.h>

#include "exceptions/MissingDefaultValueException.h"

MissingDefaultValueException::MissingDefaultValueException(const std::shared_ptr<peg::Ast> &ast, const std::string &fieldName)
	: CompilerException(ast)
	, m_fieldName(fieldName)
{
}

std::string MissingDefaultValueException::message() const
{
	return fmt::format("Field '{}' has no default value and needs to be initialized", m_fieldName);
}
