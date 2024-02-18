#include <llvm/InitializePasses.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/PassRegistry.h>

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Option.hpp>

#include <git2.h>

#include "build/build.hpp"
#include "init/init.hpp"

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
	llvm::initializeTarget(registry);

	CLI::App app("Fluenc compiler", "fcc");

	auto build_command = app.add_subcommand("build");
	auto init_command = app.add_subcommand("init");

	auto dot_cfg = build_command->add_flag("--dot-cfg", "Creates a DOT file of the resulting LLVM module");
	auto verbose = app.add_flag("--v", "Use verbose output");

	try
	{
		app.parse(argc, argv);

		if (build_command->parsed())
		{
			BuildEnvironment environment = {
				.source = std::filesystem::current_path(),
				.target = std::filesystem::current_path(),
				.root = std::filesystem::current_path(),
			};

			BuildOptions options = {
				.generateDotCfg = !dot_cfg->empty(),
				.verbose = !verbose->empty(),
			};

			return bootstrap_build(argv, environment, options);
		}

		if (init_command->parsed())
		{
			return init();
		}
	}
	catch (const CLI::ParseError &error)
	{
		return app.exit(error);
	}
}
