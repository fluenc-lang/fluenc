#ifndef INVALIDTYPEEXCEPTION_H
#define INVALIDTYPEEXCEPTION_H

#include <string>

#include "CompilerException.h"

class InvalidTypeException : public CompilerException
{
	public:
		InvalidTypeException(const std::shared_ptr<peg::Ast> &ast
			, const std::string &expectedType
			, const std::string &actualType
			);

		std::string message() const override;

	private:
		std::shared_ptr<peg::Ast> m_ast;
		std::string m_expectedType;
		std::string m_actualType;
};

#endif // INVALIDTYPEEXCEPTION_H
