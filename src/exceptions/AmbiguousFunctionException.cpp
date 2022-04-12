#include "exceptions/AmbiguousFunctionException.h"
#include "DzBaseArgument.h"
#include "Type.h"

#include "nodes/CallableNode.h"

AmbiguousFunctionException::AmbiguousFunctionException(const TokenInfo &token
	, const std::vector<CallableNode *> &functions
	, const EntryPoint &entryPoint
	)
	: CompilerException(token)
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

		stream << function->name();
		stream << "(";

		auto arguments = function->arguments();

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

				auto type = (*i)->type(*m_entryPoint);

				stream << type->name();
			}
		}

		stream << ")";
	}

	return stream.str();
}
