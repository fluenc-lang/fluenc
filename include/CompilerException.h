#ifndef COMPILEREXCEPTION_H
#define COMPILEREXCEPTION_H

#include <exception>

#include "TokenInfo.h"

class CompilerException : public std::exception
{
	public:
		CompilerException(const TokenInfo &token);

		size_t row() const;
		size_t column() const;
		size_t length() const;

		virtual std::string message() const = 0;

	private:
		TokenInfo m_token;
};

#endif // COMPILEREXCEPTION_H
