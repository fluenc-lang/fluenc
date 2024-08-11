#include <sstream>

#include "function_not_found_exception.hpp"
#include "type.hpp"

namespace fluenc::code_generation::exceptions
{
	function_not_found_exception::function_not_found_exception(
		const std::shared_ptr<peg::Ast>& ast,
		const std::string& name,
		const std::vector<const base_type*>& values,
		const tried_t& tried
	)
		: compiler_exception(ast)
		, name_(name)
		, values_(values)
		, tried_(tried)
	{
	}

	std::string function_not_found_exception::message() const
	{
		std::ostringstream stream;
		stream << "Function '" << name_ << "'";

		if (values_.size() > 0)
		{
			stream << " with signature (";

			for (auto i = values_.begin(); i != values_.end(); i++)
			{
				if (i != values_.begin())
				{
					stream << ", ";
				}

				stream << (*i)->name();
			}

			stream << ")";
		}

		stream << " was not found";

		return stream.str();
	}
}
