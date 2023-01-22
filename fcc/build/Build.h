#ifndef BUILD_H
#define BUILD_H

#include "BuildContext.h"

bool build(const BuildContext &context);

int bootstrapBuild(char **argv, const BuildEnvironment &environment, const BuildOptions &options);

#endif // BUILD_H
