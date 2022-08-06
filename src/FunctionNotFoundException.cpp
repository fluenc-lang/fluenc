#include <sstream>

#include "FunctionNotFoundException.h"

#include "values/BaseValue.h"

FunctionNotFoundException::FunctionNotFoundException(const std::shared_ptr<peg::Ast> &ast
	, const std::string &name
	, const std::vector<const Type *> &values
	)
	: CompilerException(ast)
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

		for (auto i = m_values.begin()
			; i != m_values.end()
			; i++
			)
		{
			if (i != m_values.begin())
			{
				stream << ", ";
			}

			stream << (*i)->name();
		}

		stream << ")";
	}

	stream <<  " was not found";

	return stream.str();
}
