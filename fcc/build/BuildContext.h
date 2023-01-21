#ifndef BUILDCONTEXT_H
#define BUILDCONTEXT_H

#include <llvm/Target/TargetMachine.h>

#include <clang/Driver/Driver.h>

#include "BuildOptions.h"
#include "BuildEnvironment.h"

struct BuildContext
{
	BuildOptions options;
	BuildEnvironment environment;

	clang::driver::Driver *clangDriver;

	llvm::TargetMachine *targetMachine;
	llvm::LLVMContext *llvmContext;
};

#endif // BUILDCONTEXT_H
