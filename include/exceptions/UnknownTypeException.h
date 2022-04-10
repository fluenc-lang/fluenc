#ifndef UNKNOWNTYPEEXCEPTION_H
#define UNKNOWNTYPEEXCEPTION_H

#include "CompilerException.h"

class UnknownTypeException : public CompilerException
{
	public:
		UnknownTypeException(ParserRuleContext *context, const std::string &typeName);

		std::string message() const override;

	private:
		std::string m_typeName;
};

#endif // UNKNOWNTYPEEXCEPTION_H
