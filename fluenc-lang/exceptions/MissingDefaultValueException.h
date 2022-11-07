#ifndef MISSINGDEFAULTVALUEEXCEPTION_H
#define MISSINGDEFAULTVALUEEXCEPTION_H

#include <string>

#include "CompilerException.h"

class MissingDefaultValueException : public CompilerException
{
	public:
		MissingDefaultValueException(const std::shared_ptr<peg::Ast> &ast, const std::string &fieldName);

		std::string message() const override;

	private:
		std::string m_fieldName;
};

#endif // MISSINGDEFAULTVALUEEXCEPTION_H
