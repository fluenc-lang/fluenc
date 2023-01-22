#ifndef PROJECTFILEPARSER_H
#define PROJECTFILEPARSER_H

#include <optional>
#include <filesystem>

#include "BuildConfiguration.h"

class ProjectFileParser
{
	public:
		std::optional<BuildConfiguration> parse(const std::filesystem::path &fileName);
};

#endif // PROJECTFILEPARSER_H
