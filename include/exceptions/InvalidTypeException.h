#ifndef INVALIDTYPEEXCEPTION_H
#define INVALIDTYPEEXCEPTION_H

#include "CompilerException.h"

class InvalidTypeException : public CompilerException
{
	public:
		InvalidTypeException(const TokenInfo &token
			, const std::string &expectedType
			, const std::string &actualType
			);

		std::string message() const override;

	private:
		std::string m_expectedType;
		std::string m_actualType;
};

#endif // INVALIDTYPEEXCEPTION_H
