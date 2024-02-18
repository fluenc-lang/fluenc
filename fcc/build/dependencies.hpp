#pragma once

#include "BuildConfiguration.h"

struct BuildContext;

bool build_dependencies(const BuildConfiguration &config, const BuildContext &context);
