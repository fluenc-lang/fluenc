#ifndef MISSINGFIELDEXCEPTION_H
#define MISSINGFIELDEXCEPTION_H

#include <string>

#include "CompilerException.h"

class MissingFieldException : public CompilerException
{
	public:
		MissingFieldException(const std::shared_ptr<peg::Ast> &ast
			, const std::string &typeName
			, const std::string &fieldName
			);

		std::string message() const override;

	private:
		std::string m_typeName;
		std::string m_fieldName;
};

#endif // MISSINGFIELDEXCEPTION_H
