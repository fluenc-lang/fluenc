#pragma once

#include <optional>
#include <string>
#include <vector>

#include "argument_node.hpp"
#include "expression.hpp"
#include "type_name_node.hpp"

namespace fluenc
{
	enum class function_attribute
	{
		none = 0,
		iterator,
	};

	enum class function_type
	{
		regular = 0,
		exported,
		imported
	};

	struct function_signature
	{
		std::optional<type_name_t> return_type;
		std::vector<argument_t> arguments;
		std::string name;
	};

	struct function_node
	{
		function_signature signature;
		function_type type;

		const std::shared_ptr<peg::Ast> ast;
		expression_t block;
	};

	function_attribute attribute(const function_node* node);
}
