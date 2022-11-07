#ifndef TYPEMISMATCHEXCEPTION_H
#define TYPEMISMATCHEXCEPTION_H

#include <string>

#include "CompilerException.h"

class TypeMismatchException : public CompilerException
{
	public:
		TypeMismatchException(const std::shared_ptr<peg::Ast> &ast
			, const std::string &leftTypeName
			, const std::string &rightTypeName
			);

		std::string message() const override;

	private:
		std::string m_leftTypeName;
		std::string m_rightTypeName;
};

#endif // TYPEMISMATCHEXCEPTION_H
