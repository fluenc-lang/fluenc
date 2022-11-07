#ifndef PARSEREXCEPTION_H
#define PARSEREXCEPTION_H

#include <string>

#include "CompilerException.h"

class ParserException : public CompilerException
{
	public:
		ParserException(const std::string &file, size_t line, size_t col, const std::string &message);

		std::string message() const override;

	private:
		std::string m_message;
};

#endif // PARSEREXCEPTION_H
