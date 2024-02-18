#ifndef BINARYTYPEMISMATCHEXCEPTION_H
#define BINARYTYPEMISMATCHEXCEPTION_H

#include <string>

#include "CompilerException.h"

class BinaryTypeMismatchException : public CompilerException
{
	public:
		BinaryTypeMismatchException(const std::shared_ptr<peg::Ast> &ast
			, const std::string &leftTypeName
			, const std::string &rightTypeName
			);

		std::string message() const override;

	private:
		std::string m_leftTypeName;
		std::string m_rightTypeName;
};

#endif // BINARYTYPEMISMATCHEXCEPTION_H
