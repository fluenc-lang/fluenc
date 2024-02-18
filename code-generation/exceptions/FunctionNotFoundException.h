#pragma once

#include "CompilerException.h"

#include <vector>

namespace fluenc
{
	struct function_node;
}

class Type;

using tried_t = std::vector<std::pair<int8_t, const fluenc::function_node *>>;

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
