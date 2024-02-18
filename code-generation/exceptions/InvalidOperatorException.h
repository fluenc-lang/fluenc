#ifndef INVALIDOPERATOREXCEPTION_H
#define INVALIDOPERATOREXCEPTION_H

#include <string>

#include "CompilerException.h"

class InvalidOperatorException : public CompilerException
{
	public:
		InvalidOperatorException(const std::shared_ptr<peg::Ast> &ast, const std::string &op, const std::string &typeName);

		std::string message() const override;

	private:
		std::string m_op;
		std::string m_typeName;
};

#endif // INVALIDOPERATOREXCEPTION_H
