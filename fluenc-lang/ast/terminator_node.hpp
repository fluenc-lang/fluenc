#pragma once

namespace fluenc
{
	struct terminator_node
	{
		static terminator_node* instance()
		{
			static terminator_node instance;

			return &instance;
		}
	};
}
