#pragma once

#include "BuildContext.h"

bool build(const BuildContext &context);

int bootstrap_build(char** argv, const BuildEnvironment& environment, const BuildOptions& options);
