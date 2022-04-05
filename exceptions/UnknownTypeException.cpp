#include "UnknownTypeException.h"

UnknownTypeException::UnknownTypeException(ParserRuleContext *context, const std::string &typeName)
	: CompilerException(context)
	, m_typeName(typeName)
{
}

std::string UnknownTypeException::message() const
{
	std::stringstream stream;
	stream << "Type '" << m_typeName << "' was not found";

	return  stream.str();
}
