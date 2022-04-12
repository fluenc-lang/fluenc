#include <sstream>

#include "FunctionNotFoundException.h"

#include "values/BaseValue.h"

FunctionNotFoundException::FunctionNotFoundException(const TokenInfo &token
	, const std::string &name
	, const Stack &values
	)
	: CompilerException(token)
	, m_name(name)
	, m_values(values)
{
}

std::string FunctionNotFoundException::message() const
{
	std::ostringstream stream;
	stream << "Function '" << m_name << "'";

	if (m_values.size() > 0)
	{
		stream << " with signature (";

		for (auto i = m_values.rbegin()
			; i != m_values.rend()
			; i++
			)
		{
			if (i != m_values.rbegin())
			{
				stream << ", ";
			}

			auto type = (*i)->type();

			stream << type->name();
		}

		stream << ")";
	}

	stream <<  " was not found";

	return stream.str();
}
