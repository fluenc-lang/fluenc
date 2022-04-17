#ifndef MISSINGDEFAULTVALUEEXCEPTION_H
#define MISSINGDEFAULTVALUEEXCEPTION_H

#include "CompilerException.h"

class MissingDefaultValueException : public CompilerException
{
	public:
		MissingDefaultValueException(const TokenInfo &token, const std::string &fieldName);

		std::string message() const override;

	private:
		std::string m_fieldName;
};

#endif // MISSINGDEFAULTVALUEEXCEPTION_H
