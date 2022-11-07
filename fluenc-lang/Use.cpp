#include "Use.h"

Use::Use(const std::shared_ptr<peg::Ast> &ast, const std::string &fileName)
	: m_ast(ast)
	, m_fileName(fileName)
{
}

std::shared_ptr<peg::Ast> Use::ast() const
{
	return m_ast;
}

std::string Use::fileName() const
{
	return m_fileName;
}
