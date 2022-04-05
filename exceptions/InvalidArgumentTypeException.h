#ifndef INVALIDARGUMENTTYPEEXCEPTION_H
#define INVALIDARGUMENTTYPEEXCEPTION_H

#include "CompilerException.h"

class InvalidArgumentTypeException : public CompilerException
{
	public:
		InvalidArgumentTypeException(ParserRuleContext *context);

		std::string message() const override;
};

#endif // INVALIDARGUMENTTYPEEXCEPTION_H
