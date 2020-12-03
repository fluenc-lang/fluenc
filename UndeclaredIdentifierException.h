#ifndef UNDECLAREDIDENTIFIEREXCEPTION_H
#define UNDECLAREDIDENTIFIEREXCEPTION_H

#include "CompilerException.h"

class UndeclaredIdentifierException : public CompilerException
{
	public:
		UndeclaredIdentifierException(ParserRuleContext *context, const std::string &identifier);

		std::string message() const override;

	private:
		std::string m_identifier;
};


#endif // UNDECLAREDIDENTIFIEREXCEPTION_H
