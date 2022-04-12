#ifndef AMBIGUOUSFUNCTIONEXCEPTION_H
#define AMBIGUOUSFUNCTIONEXCEPTION_H

#include "CompilerException.h"

class CallableNode;
class EntryPoint;

class AmbiguousFunctionException : public CompilerException
{
	public:
		AmbiguousFunctionException(const TokenInfo &token
			, const std::vector<CallableNode *> &functions
			, const EntryPoint &entryPoint
			);

		std::string message() const override;

	private:
		std::vector<CallableNode *> m_functions;

		const EntryPoint *m_entryPoint;
};

#endif // AMBIGUOUSFUNCTIONEXCEPTION_H
