#ifndef INVALIDFUNCTIONPOINTERTYPEEXCEPTION_H
#define INVALIDFUNCTIONPOINTERTYPEEXCEPTION_H

#include <string>

#include "CompilerException.h"

class InvalidFunctionPointerTypeException : public CompilerException
{
	public:
		InvalidFunctionPointerTypeException(const std::shared_ptr<peg::Ast> &ast, const std::string &name);

		std::string message() const override;

	private:
		std::string m_name;
};

#endif // INVALIDFUNCTIONPOINTERTYPEEXCEPTION_H
