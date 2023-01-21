#ifndef PROJECTFILEPARSER_H
#define PROJECTFILEPARSER_H

#include <optional>

#include "BuildConfiguration.h"

class ProjectFileParser
{
	public:
		std::optional<BuildConfiguration> parse(const std::string &fileName);
};

#endif // PROJECTFILEPARSER_H
