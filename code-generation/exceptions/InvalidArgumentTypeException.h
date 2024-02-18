#ifndef INVALIDARGUMENTTYPEEXCEPTION_H
#define INVALIDARGUMENTTYPEEXCEPTION_H

#include "CompilerException.h"

class InvalidArgumentTypeException : public CompilerException
{
	public:
		InvalidArgumentTypeException(const std::shared_ptr<peg::Ast> &ast);

		std::string message() const override;
};

#endif // INVALIDARGUMENTTYPEEXCEPTION_H
