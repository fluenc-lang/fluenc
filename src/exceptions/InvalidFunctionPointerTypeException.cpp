#include <sstream>

#include "exceptions/InvalidFunctionPointerTypeException.h"

InvalidFunctionPointerTypeException::InvalidFunctionPointerTypeException(const std::shared_ptr<peg::Ast> &ast, const std::string &name)
	: CompilerException(ast)
	, m_name(name)
{
}

std::string InvalidFunctionPointerTypeException::message() const
{
	std::stringstream stream;
	stream << "'" << m_name << "' is not of function pointer type";

	return stream.str();
}
