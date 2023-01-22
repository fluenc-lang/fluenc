#ifndef BUILDENVIRONMENT_H
#define BUILDENVIRONMENT_H

#include <filesystem>

struct BuildEnvironment
{
	std::filesystem::path source;
	std::filesystem::path target;
	std::filesystem::path root;
};

#endif // BUILDENVIRONMENT_H
