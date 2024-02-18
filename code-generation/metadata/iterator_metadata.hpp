#pragma once

#include "value_metadata.hpp"

struct iterator_metadata : public value_metadata
{
	std::string name() const;
};
