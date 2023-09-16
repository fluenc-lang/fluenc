#ifndef FUNCTIONNOTFOUNDEXCEPTION_H
#define FUNCTIONNOTFOUNDEXCEPTION_H

#include "CompilerException.h"
#include "Stack.h"

class CallableNode;

using tried_t = std::vector<std::pair<int8_t, const CallableNode *>>;

class FunctionNotFoundException : public CompilerException
{
	public:
		FunctionNotFoundException(const std::shared_ptr<peg::Ast> &ast
			, const std::string &name
			, const std::vector<const Type *> &values
			, const tried_t& tried
			);

		std::string message() const override;

	private:
		std::string m_name;
		std::vector<const Type *> m_values;

		tried_t m_tried;
};

#endif // FUNCTIONNOTFOUNDEXCEPTION_H
