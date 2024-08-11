#pragma once

#include "value_metadata.hpp"

struct without_value_metadata : public value_metadata
{
	std::string name() const override;
};
