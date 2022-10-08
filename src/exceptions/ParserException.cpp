#include <vector>

#include "../peglib.h"

#include "exceptions/ParserException.h"

ParserException::ParserException(const std::string &file, size_t line, size_t col, const std::string &message)
	: CompilerException(std::make_shared<peg::Ast>(file.c_str(), line, col, "dummy", std::vector<std::shared_ptr<peg::Ast>>()))
	, m_message(message)
{
}

std::string ParserException::message() const
{
	return m_message;
}
