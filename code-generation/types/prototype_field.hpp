#pragma once

#include <string>

namespace fluenc::code_generation
{
	struct base_value;
	struct base_type;

	namespace types
	{
		struct prototype_field
		{
			std::string name;

			const base_value* default_value;
			const base_type* type;
		};
	}
}
