#ifndef PROJECTFILEPARSER_H
#define PROJECTFILEPARSER_H

#include "BuildConfiguration.h"

class ProjectFileParser
{
	public:
		BuildConfiguration parse(const std::string &fileName);
};

#endif // PROJECTFILEPARSER_H
