#ifndef BUILDCONFIGURATION_H
#define BUILDCONFIGURATION_H

#include <string>
#include <vector>

struct BuildConfiguration
{
	std::string type;
	std::string target;

	std::vector<std::string> libs;
};

#endif // BUILDCONFIGURATION_H
