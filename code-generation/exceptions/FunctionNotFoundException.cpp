#include <sstream>

#include "FunctionNotFoundException.h"
#include "BaseValue.h"
#include "Type.h"

FunctionNotFoundException::FunctionNotFoundException(const std::shared_ptr<peg::Ast> &ast
	, const std::string &name
	, const std::vector<const Type *> &values
	, const tried_t &tried
	)
	: CompilerException(ast)
	, m_name(name)
	, m_values(values)
	, m_tried(tried)
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
