#pragma once

#include <string>

class value_metadata
{
public:
	virtual std::string name() const = 0;
};
