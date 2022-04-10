#ifndef MISSINGTYPEDECLARATIONEXCEPTION_H
#define MISSINGTYPEDECLARATIONEXCEPTION_H

#include "CompilerException.h"

class MissingTypeDeclarationException : public CompilerException
{
	public:
		MissingTypeDeclarationException(ParserRuleContext *context
			, const std::string &typeName
			, const std::string &fieldName
			);

		std::string message() const override;

	private:
		std::string m_typeName;
		std::string m_fieldName;
};

#endif // MISSINGTYPEDECLARATIONEXCEPTION_H
