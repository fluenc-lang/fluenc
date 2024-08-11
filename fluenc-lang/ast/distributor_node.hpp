#pragma once

#include "visitable.hpp"

namespace fluenc
{
	struct distributor_node
	{
		visitable_t subject;
		visitable_t consumer;
	};
}
