#pragma once

#include "prototype_field.hpp"

#include <vector>

namespace fluenc
{
	struct struct_node;

	namespace code_generation
	{
		struct entry_point;
		struct emitter;

		namespace types
		{
			std::pair<entry_point, std::vector<prototype_field>> default_fields(
				const entry_point& entryPoint,
				const emitter& visitor,
				const struct_node* prototype
			);
		}
	}
}
