#include <sstream>

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
	std::ostringstream stream;
	stream << "Field '";
	stream << m_fieldName;
	stream << "' in type '";
	stream << m_typeName;
	stream << "' does not exist";

	return stream.str();
}
