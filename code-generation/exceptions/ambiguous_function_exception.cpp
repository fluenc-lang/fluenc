#include <sstream>

#include "entry_point.hpp"
#include "type.hpp"

#include "ast/function_node.hpp"

#include "exceptions/ambiguous_function_exception.hpp"

namespace fluenc::code_generation::exceptions
{
	ambiguous_function_exception::ambiguous_function_exception(
		const std::shared_ptr<peg::Ast>& ast,
		const std::vector<fluenc::function_node*>& functions,
		const entry_point& entryPoint
	)
		: compiler_exception(ast)
		, functions_(functions)
		, entry_point_(new entry_point(entryPoint))
	{
	}

	std::string ambiguous_function_exception::message() const
	{
		std::ostringstream stream;
		stream << "The call is ambiguous between ";

		for (auto i = begin(functions_); i != end(functions_); i++)
		{
			if (i != begin(functions_))
			{
				stream << " and ";
			}

			auto function = *i;

			stream << function->signature.name;
			stream << "(";

			auto arguments = function->signature.arguments;

			if (arguments.size() > 0)
			{
				for (auto i = begin(arguments); i != end(arguments); i++)
				{
					if (i != begin(arguments))
					{
						stream << ", ";
					}

					auto type = arg_type(*i, *entry_point_);

					stream << type->name();
				}
			}

			stream << ")";
		}

		return stream.str();
	}
}
