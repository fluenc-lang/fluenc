#ifndef BUILDCONFIGURATION_H
#define BUILDCONFIGURATION_H

#include <string>
#include <unordered_map>
#include <vector>

struct BuildConfiguration
{
	std::string type;
	std::string target;

	std::vector<std::string> libs;
	std::vector<std::string> modules;

	std::unordered_map<std::string, std::string> repos;
};

#endif // BUILDCONFIGURATION_H
