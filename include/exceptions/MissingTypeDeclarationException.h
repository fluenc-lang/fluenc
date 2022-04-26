#ifndef MISSINGTYPEDECLARATIONEXCEPTION_H
#define MISSINGTYPEDECLARATIONEXCEPTION_H

#include <string>

#include "CompilerException.h"

class MissingTypeDeclarationException : public CompilerException
{
	public:
		MissingTypeDeclarationException(const std::shared_ptr<peg::Ast> &ast
			, const std::string &typeName
			, const std::string &fieldName
			);

		std::string message() const override;

	private:
		std::string m_typeName;
		std::string m_fieldName;
};

#endif // MISSINGTYPEDECLARATIONEXCEPTION_H
