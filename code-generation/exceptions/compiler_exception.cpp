#include "peglib.h"

#include "compiler_exception.hpp"

compiler_exception::compiler_exception(const std::shared_ptr<peg::Ast>& ast)
	: m_ast(ast)
{
}

size_t compiler_exception::row() const
{
	return m_ast->line;
}

size_t compiler_exception::column() const
{
	return m_ast->column - 1;
}

size_t compiler_exception::length() const
{
	return m_ast->length;
}

std::string compiler_exception::file() const
{
	return m_ast->path;
}
