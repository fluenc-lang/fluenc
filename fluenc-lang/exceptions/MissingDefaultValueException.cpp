#include <sstream>

#include "exceptions/MissingDefaultValueException.h"

MissingDefaultValueException::MissingDefaultValueException(const std::shared_ptr<peg::Ast> &ast, const std::string &fieldName)
	: CompilerException(ast)
	, m_fieldName(fieldName)
{
}

std::string MissingDefaultValueException::message() const
{
	std::ostringstream stream;
	stream << "Field '";
	stream << m_fieldName;
	stream << "' has no default value and needs to be initialized";

	return stream.str();
}
