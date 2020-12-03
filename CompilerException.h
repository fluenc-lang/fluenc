#ifndef COMPILEREXCEPTION_H
#define COMPILEREXCEPTION_H

#include <exception>

#include "antlr4-runtime/dzBaseVisitor.h"
#include "antlr4-runtime/dzLexer.h"
#include "antlr4-runtime/dzParser.h"

using namespace antlr4;

class CompilerException : public std::exception
{
	public:
		CompilerException(ParserRuleContext *context);

		int row() const;
		int column() const;
		int length() const;

		virtual std::string message() const = 0;

	private:
		ParserRuleContext *m_context;
};

#endif // COMPILEREXCEPTION_H
