#include "UnknownTypeException.h"

UnknownTypeException::UnknownTypeException(ParserRuleContext *context, const std::string &typeName)
	: CompilerException(context)
	, m_typeName(typeName)
{
}

std::string UnknownTypeException::message() const
{
	std::stringstream stream;
	stream << "Unknown type name '" << m_typeName << "'";

	return  stream.str();
}
