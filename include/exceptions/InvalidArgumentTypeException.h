#ifndef INVALIDARGUMENTTYPEEXCEPTION_H
#define INVALIDARGUMENTTYPEEXCEPTION_H

#include "CompilerException.h"

class InvalidArgumentTypeException : public CompilerException
{
	public:
		InvalidArgumentTypeException(const TokenInfo &token);

		std::string message() const override;
};

#endif // INVALIDARGUMENTTYPEEXCEPTION_H
