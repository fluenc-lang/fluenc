#ifndef COMPILEREXCEPTION_H
#define COMPILEREXCEPTION_H

#include <exception>

#include "antlr4-runtime/fluencBaseVisitor.h"
#include "antlr4-runtime/fluencLexer.h"
#include "antlr4-runtime/fluencParser.h"

using namespace antlr4;

struct TokenInfo
{
	size_t row;
	size_t column;
	size_t length;
};

class CompilerException : public std::exception
{
	public:
		CompilerException(ParserRuleContext *context);

		size_t row() const;
		size_t column() const;
		size_t length() const;

		virtual std::string message() const = 0;

	private:
		TokenInfo m_tokenInfo;
};

#endif // COMPILEREXCEPTION_H
