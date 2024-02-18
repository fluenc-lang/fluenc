#include <sstream>

#include "Type.h"
#include "EntryPoint.h"

#include "ast/function_node.hpp"

#include "exceptions/AmbiguousFunctionException.h"

AmbiguousFunctionException::AmbiguousFunctionException(const std::shared_ptr<peg::Ast> &ast
	, const std::vector<fluenc::function_node*> &functions
	, const EntryPoint &entryPoint
	)
	: CompilerException(ast)
	, m_functions(functions)
	, m_entryPoint(new EntryPoint(entryPoint))
{
}

std::string AmbiguousFunctionException::message() const
{
	std::ostringstream stream;
	stream << "The call is ambiguous between ";

	for (auto i = begin(m_functions)
		; i != end(m_functions)
		; i++
		)
	{
		if (i != begin(m_functions))
		{
			stream << " and ";
		}

		auto function = *i;

		stream << function->signature.name;
		stream << "(";

		auto arguments = function->signature.arguments;

		if (arguments.size() > 0)
		{
			for (auto i = begin(arguments)
				; i != end(arguments)
				; i++
				)
			{
				if (i != begin(arguments))
				{
					stream << ", ";
				}

				auto type = arg_type(*i, *m_entryPoint);

				stream << type->name();
			}
		}

		stream << ")";
	}

	return stream.str();
}
