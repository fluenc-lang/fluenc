#include "peglib.h"

#include "CompilerException.h"

CompilerException::CompilerException(const std::shared_ptr<peg::Ast> &ast)
	: m_ast(ast)
{
}

size_t CompilerException::row() const
{
	return m_ast->line;
}

size_t CompilerException::column() const
{
	return m_ast->column;
}

size_t CompilerException::length() const
{
	return m_ast->length;
}
