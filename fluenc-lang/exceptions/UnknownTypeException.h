#ifndef UNKNOWNTYPEEXCEPTION_H
#define UNKNOWNTYPEEXCEPTION_H

#include <string>

#include "CompilerException.h"

class UnknownTypeException : public CompilerException
{
	public:
		UnknownTypeException(const std::shared_ptr<peg::Ast> &ast, const std::string &typeName);

		std::string message() const override;

	private:
		std::string m_typeName;
};

#endif // UNKNOWNTYPEEXCEPTION_H
