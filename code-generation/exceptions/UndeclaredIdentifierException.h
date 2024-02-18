#ifndef UNDECLAREDIDENTIFIEREXCEPTION_H
#define UNDECLAREDIDENTIFIEREXCEPTION_H

#include <string>

#include "CompilerException.h"

class UndeclaredIdentifierException : public CompilerException
{
	public:
		UndeclaredIdentifierException(const std::shared_ptr<peg::Ast> &ast, const std::string &identifier);

		std::string message() const override;

	private:
		std::string m_identifier;
};


#endif // UNDECLAREDIDENTIFIEREXCEPTION_H
