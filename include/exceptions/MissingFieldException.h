#ifndef MISSINGFIELDEXCEPTION_H
#define MISSINGFIELDEXCEPTION_H

#include "CompilerException.h"

class MissingFieldException : public CompilerException
{
	public:
		MissingFieldException(const TokenInfo &token
			, const std::string &typeName
			, const std::string &fieldName
			);

		std::string message() const override;

	private:
		std::string m_typeName;
		std::string m_fieldName;
};

#endif // MISSINGFIELDEXCEPTION_H
