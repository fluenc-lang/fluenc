#pragma once

#include <any>
#include <string>
#include <vector>

namespace fluenc
{
	struct namespace_t
	{
		std::vector<std::any> children;
		std::string name;
	};
}
