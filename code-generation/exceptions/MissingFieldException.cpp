#include <sstream>

#include <fmt/core.h>

#include "exceptions/MissingFieldException.h"

MissingFieldException::MissingFieldException(const std::shared_ptr<peg::Ast> &ast
	, const std::string &typeName
	, const std::string &fieldName
	)
	: CompilerException(ast)
	, m_typeName(typeName)
	, m_fieldName(fieldName)
{
}

std::string MissingFieldException::message() const
{
	return fmt::format("Field '{}' in type '{}' does not exist", m_fieldName, m_typeName);
}
