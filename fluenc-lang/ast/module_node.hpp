#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "expression.hpp"

namespace fluenc
{
	namespace code_generation
	{
		struct base_value;
	}

	struct struct_node;

	struct module_node
	{
		std::vector<const function_node*> roots;
		std::multimap<std::string, function_node*> functions;
		std::map<std::string, const code_generation::base_value*> locals;
		std::map<std::string, expression_t> globals;
		std::map<std::string, struct_node*> types;

		static module_node merge(const module_node& left, const module_node& right, bool skip_roots)
		{
			module_node target;

			if (!skip_roots)
			{
				std::copy(begin(left.roots), end(left.roots), back_inserter(target.roots));
				std::copy(begin(right.roots), end(right.roots), back_inserter(target.roots));
			}

			std::copy(begin(left.functions), end(left.functions), inserter(target.functions, begin(target.functions)));
			std::copy(begin(right.functions), end(right.functions), inserter(target.functions, begin(target.functions)));

			std::copy(begin(left.locals), end(left.locals), inserter(target.locals, begin(target.locals)));
			std::copy(begin(right.locals), end(right.locals), inserter(target.locals, begin(target.locals)));

			std::copy(begin(left.globals), end(left.globals), inserter(target.globals, begin(target.globals)));
			std::copy(begin(right.globals), end(right.globals), inserter(target.globals, begin(target.globals)));

			std::copy(begin(left.types), end(left.types), inserter(target.types, begin(target.types)));
			std::copy(begin(right.types), end(right.types), inserter(target.types, begin(target.types)));

			return target;
		}
	};
}
