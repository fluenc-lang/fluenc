#include "type.hpp"

namespace fluenc::code_generation
{
	std::string base_type::full_name() const
	{
		return name();
	}

	operator_set* base_type::operators() const
	{
		return nullptr;
	}
}
