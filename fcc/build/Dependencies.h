#ifndef DEPENDENCIES_H
#define DEPENDENCIES_H

#include "BuildConfiguration.h"

struct BuildContext;

bool buildDependencies(const BuildConfiguration &config, const BuildContext &context);

#endif // DEPENDENCIES_H
