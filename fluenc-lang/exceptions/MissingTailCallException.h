#ifndef MISSINGTAILCALLEXCEPTION_H
#define MISSINGTAILCALLEXCEPTION_H

#include <string>

#include "CompilerException.h"

class MissingTailCallException : public CompilerException
{
	public:
		MissingTailCallException(const std::shared_ptr<peg::Ast> &ast);

		std::string message() const override;
};

#endif // MISSINGTAILCALLEXCEPTION_H
