#pragma once

#include <vector>

#include "CompilerException.h"

namespace fluenc
{
	struct function_node;
}

class EntryPoint;

class AmbiguousFunctionException : public CompilerException
{
	public:
		AmbiguousFunctionException(const std::shared_ptr<peg::Ast> &ast
			, const std::vector<fluenc::function_node *> &functions
			, const EntryPoint &entryPoint
			);

		std::string message() const override;

	private:
		std::vector<fluenc::function_node*> m_functions;

		const EntryPoint *m_entryPoint;
};
