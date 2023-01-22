#include <llvm/InitializePasses.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/PassRegistry.h>

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Option.hpp>

#include <git2.h>

#include "build/Build.h"
#include "init/Init.h"

int main(int argc, char **argv)
{
	git_libgit2_init();

	llvm::InitializeAllTargetInfos();
	llvm::InitializeAllTargets();
	llvm::InitializeAllTargetMCs();
	llvm::InitializeAllAsmParsers();
	llvm::InitializeAllAsmPrinters();

	auto &registry = *llvm::PassRegistry::getPassRegistry();

	llvm::initializeCore(registry);
	llvm::initializeScalarOpts(registry);
	llvm::initializeVectorization(registry);
	llvm::initializeIPO(registry);
	llvm::initializeAnalysis(registry);
	llvm::initializeTransformUtils(registry);
	llvm::initializeInstCombine(registry);
	llvm::initializeInstrumentation(registry);
	llvm::initializeTarget(registry);

	CLI::App app("Fluenc compiler", "fcc");

	auto buildCommand = app.add_subcommand("build");
	auto initCommand = app.add_subcommand("init");

	auto dotCfg = buildCommand->add_flag("--dot-cfg", "Creates a DOT file of the resulting LLVM module");
	auto verbose = app.add_flag("--v", "Use verbose output");

	try
	{
		app.parse(argc, argv);

		if (buildCommand->parsed())
		{
			BuildEnvironment environment = {
				.source = std::filesystem::current_path(),
				.target = std::filesystem::current_path(),
				.root = std::filesystem::current_path(),
			};

			BuildOptions options = {
				.generateDotCfg = !dotCfg->empty(),
				.verbose = !verbose->empty()
			};

			return bootstrapBuild(argv, environment, options);
		}

		if (initCommand->parsed())
		{
			return init();
		}
	}
	catch (const CLI::ParseError &error)
	{
		return app.exit(error);
	}
}
