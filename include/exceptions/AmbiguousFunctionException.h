#ifndef AMBIGUOUSFUNCTIONEXCEPTION_H
#define AMBIGUOUSFUNCTIONEXCEPTION_H

#include <vector>

#include "CompilerException.h"

class CallableNode;
class EntryPoint;

class AmbiguousFunctionException : public CompilerException
{
	public:
		AmbiguousFunctionException(const std::shared_ptr<peg::Ast> &ast
			, const std::vector<CallableNode *> &functions
			, const EntryPoint &entryPoint
			);

		std::string message() const override;

	private:
		std::vector<CallableNode *> m_functions;

		const EntryPoint *m_entryPoint;
};

#endif // AMBIGUOUSFUNCTIONEXCEPTION_H
