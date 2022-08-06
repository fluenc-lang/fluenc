#include <stdio.h>

#include <string_view>
#include <filesystem>
#include <fstream>

#include <lld/Common/Driver.h>

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Analysis/CFGPrinter.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Config/llvm-config.h>
#if LLVM_VERSION_MAJOR == 14
#include <llvm/MC/TargetRegistry.h>
#else
#include <llvm/Support/TargetRegistry.h>
#endif
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetOptions.h>

#include <llvm/Support/Host.h>
#include <llvm/Target/TargetMachine.h>

#include "peglib.h"
#include "incbin.h"

#include "ProjectFileParser.h"
#include "CompilerException.h"
#include "Visitor.h"
#include "ModuleInfo.h"
#include "Emitter.h"

#include "nodes/CallableNode.h"

INCTXT(Grammar, "fluenc.peg");

struct CompilerJob
{
	std::string name;
	std::string objectFile;
	std::string sourceFile;

	std::filesystem::file_time_type objectFileTime;
	std::filesystem::file_time_type sourceFileTime;

	ModuleInfo module;
};

CompilerJob createJob(const ModuleInfo &module, const std::filesystem::path &stem)
{
	auto name = stem.filename();

	auto objectFile = stem.string() + ".o";
	auto sourceFile = stem.string() + ".fc";

	if (std::filesystem::exists(objectFile))
	{
		return
		{
			name,
			objectFile,
			sourceFile,
			std::filesystem::last_write_time(objectFile),
			std::filesystem::last_write_time(sourceFile),
			module
		};
	}

	return
	{
		name,
		objectFile,
		sourceFile,
		std::filesystem::file_time_type(),
		std::filesystem::last_write_time(sourceFile),
		module,
	};
}

ModuleInfo analyze(const std::string &file, peg::parser &parser)
{
	std::ifstream stream(file);
	std::stringstream buffer;
	buffer << stream.rdbuf();

	auto source = new std::string(buffer.str());

	std::shared_ptr<peg::Ast> ast;

	parser.parse(*source, ast, file.c_str());

	Visitor visitor(std::vector<std::string>(), nullptr, nullptr, nullptr, nullptr);

	auto moduleInfo = visitor
		.visit(ast);

	return moduleInfo;
}

std::string translateUse(const std::string &use, const CompilerJob &job)
{
	auto sourceFilePath = std::filesystem::path(job.sourceFile);
	auto relativeUsePath = sourceFilePath.parent_path() / use;

	return relativeUsePath.lexically_normal();
}

bool isStale(std::unordered_set<std::string> &processed
	, const CompilerJob &job
	, const CompilerJob &current
	, const std::unordered_map<std::string, CompilerJob> jobs
	)
{
	std::cout << job.sourceFile << " depends on " << current.sourceFile << std::endl;

	auto [_, success] = processed.insert(current.sourceFile);

	if (!success)
	{
		return false;
	}

	if (current.sourceFileTime > job.objectFileTime)
	{
		std::cout << current.sourceFile << " is more recent than " << job.sourceFile << std::endl;

		return true;
	}

	for (auto &use : current.module.uses)
	{
		auto translated = translateUse(use, current);
		auto childJob = jobs.find(translated);

		if (childJob == jobs.end())
		{
			throw new std::exception(); // TODO
		}

		if (isStale(processed, job, childJob->second, jobs))
		{
			return true;
		}
	}

	return false;
}

bool isStale(const CompilerJob &job
	, const std::unordered_map<std::string, CompilerJob> jobs
	)
{
	if (job.objectFileTime.time_since_epoch() == std::filesystem::file_time_type::duration::zero())
	{
		std::cout << "No output found for " << job.sourceFile << std::endl;

		return true;
	}

	std::unordered_set<std::string> processed;

	return isStale(processed, job, job, jobs);
}

ModuleInfo processUses(std::unordered_set<std::string> &processed
	, const CompilerJob &job
	, const std::unordered_map<std::string, CompilerJob> jobs
	)
{
	std::cout << "Adding info from " << job.sourceFile << std::endl;

	auto [_, success] = processed.insert(job.sourceFile);

	if (!success)
	{
		return ModuleInfo();
	}

	auto target = job.module;

	for (auto &use : job.module.uses)
	{
		auto translated = translateUse(use, job);
		auto childJob = jobs.find(translated);

		if (childJob == jobs.end())
		{
			throw new std::exception(); // TODO
		}

		auto source = processUses(processed, childJob->second, jobs);

		std::copy(begin(source.functions), end(source.functions), std::inserter(target.functions, begin(target.functions)));
		std::copy(begin(source.locals), end(source.locals), std::inserter(target.locals, begin(target.locals)));
		std::copy(begin(source.globals), end(source.globals), std::inserter(target.globals, begin(target.globals)));
		std::copy(begin(source.types), end(source.types), std::inserter(target.types, begin(target.types)));
	}

	return target;
}

ModuleInfo processUses(const CompilerJob &job
	, const std::unordered_map<std::string, CompilerJob> jobs
	)
{
	std::unordered_set<std::string> processed;

	return processUses(processed, job, jobs);
}

int main(int argc, char **argv)
{
	llvm::InitializeAllTargetInfos();
	llvm::InitializeAllTargets();
	llvm::InitializeAllTargetMCs();
	llvm::InitializeAllAsmParsers();
	llvm::InitializeAllAsmPrinters();

	auto llvmContext = std::make_unique<llvm::LLVMContext>();

	std::string errors;

	auto targetTriple = llvm::sys::getDefaultTargetTriple();
	auto target = llvm::TargetRegistry::lookupTarget(targetTriple, errors);

	if (!target)
	{
		llvm::errs() << errors;

		return 1;
	}

	auto relocModel = llvm::Optional<llvm::Reloc::Model>();
	auto targetMachine = target->createTargetMachine(targetTriple, "generic", "", llvm::TargetOptions(), relocModel);
	auto dataLayout = targetMachine->createDataLayout();

	ProjectFileParser projectFileParser;

	auto configuration = projectFileParser.parse("project.toml");

	peg::parser parser(gGrammarData);

	parser.log = [](size_t line, size_t col, const std::string &msg)
	{
		std::cerr << line << ":" << col << ": " << msg << "\n";
	};

	parser.enable_ast();
	parser.enable_packrat_parsing();

	std::unordered_map<std::string, CompilerJob> jobs;

	auto workingDirectory = std::filesystem::current_path();

	for (const auto &entry : std::filesystem::recursive_directory_iterator(workingDirectory))
	{
		auto path = entry.path();

		if (path.extension() != ".fc")
		{
			std::cout << "Skipping file " << path << std::endl;

			continue;
		}

		std::cout << "Adding file " << path << std::endl;

		auto relative = std::filesystem::relative(path);

		auto relativeStem = relative.parent_path() / relative.stem();

		auto module = analyze(relative, parser);
		auto job = createJob(module, relativeStem);

		jobs.insert({ job.sourceFile, job});
	}

	std::vector<std::string> objectFiles;

	auto success = std::accumulate(begin(jobs), end(jobs), true, [&](auto result, auto pair)
	{
		auto &[_, job] = pair;

		if (job.module.roots.empty())
		{
			std::cout << "File " << job.sourceFile << " has no roots, skipping" << std::endl;

			return result;
		}

		if (isStale(job, jobs))
		{
			auto module = processUses(job, jobs);

			std::cout << "Building " << job.sourceFile << "..." << std::endl;

			auto llvmModule = std::make_unique<llvm::Module>(job.name, *llvmContext);

			Stack values;

			EntryPoint entryPoint(0
				, -1
				, nullptr
				, nullptr
				, nullptr
				, nullptr
				, nullptr
				, llvmModule.get()
				, llvmContext.get()
				, "term"
				, module.functions
				, module.locals
				, module.globals
				, module.types
				, values
				, nullptr
				);

			try
			{
				Emitter emitter;

				for (auto root : module.roots)
				{
					root->accept(emitter, entryPoint, values);
				}

				llvmModule->print(llvm::errs(), nullptr);
				llvmModule->setDataLayout(dataLayout);

				if (verifyModule(*llvmModule, &llvm::errs()))
				{
					throw new std::exception();
				}

				std::error_code error;

				llvm::raw_fd_ostream destination(job.objectFile, error, llvm::sys::fs::OF_None);

				llvm::legacy::PassManager passManager;

				if (argc > 1)
				{
					if (!strcmp(argv[1], "-dot-cfg"))
					{
						passManager.add(llvm::createCFGPrinterLegacyPassPass());
					}
				}

				targetMachine->addPassesToEmitFile(passManager, destination, nullptr, llvm::CGFT_ObjectFile);

				passManager.run(*llvmModule);

				destination.flush();
			}
			catch (CompilerException *exception)
			{
				std::cout << exception->file() << ":" << exception->row() << ":" << exception->column() << ": error: " << exception->message() << std::endl;
				std::cout << std::setw(5) << exception->row() << " | ";

				std::ifstream stream;

				stream.open(exception->file());

				std::string line;

				for (size_t i = 0u; i < exception->row(); i++)
				{
					std::getline(stream, line);
				}

				auto trimmed = std::find_if(begin(line), end(line), [](unsigned char ch)
				{
					return !std::isspace(ch);
				});

				for (auto i = trimmed; i != end(line); i++)
				{
					std::cout << *i;
				}

				std::cout << std::endl;
				std::cout << std::setw(8) << "| ";

				for (auto i = trimmed; i != end(line); i++)
				{
					auto tokenSelector = [&]
					{
						if (i == begin(line) + exception->column())
						{
							return "^";
						}

						if (i < begin(line) + exception->column())
						{
							return " ";
						}

						if (i > begin(line) + exception->column() + exception->length())
						{
							return " ";
						}

						return "~";
					};

					std::cout << tokenSelector();
				}

				std::cout << std::endl;

				return false;
			}
		}

		objectFiles.push_back(job.objectFile);

		return result;
	});

	if (!success)
	{
		std::cout << "Build failed" << std::endl;

		return 1;
	}

	std::cout << "Linking " << configuration.target << "..." << std::endl;

	std::vector<std::string> linkerArguments =
	{
		"fcc",
		"-dynamic-linker",
		"/lib64/ld-linux-x86-64.so.2",
		"/usr/lib/crt1.o",
		"/usr/lib/crti.o",
		"/usr/lib/crtn.o",
		"-L/usr/lib",
		"-lc",
	};

	linkerArguments.insert(end(linkerArguments), begin(configuration.libs), end(configuration.libs));
	linkerArguments.insert(end(linkerArguments), begin(objectFiles), end(objectFiles));

	linkerArguments.push_back("-o");
	linkerArguments.push_back(configuration.target);

	std::vector<const char *> arguments;

	std::transform(begin(linkerArguments), end(linkerArguments), std::back_inserter(arguments), [](auto argument)
	{
		return strcpy(new char[argument.size() + 1], argument.c_str());
	});

#if LLVM_VERSION_MAJOR == 14
    lld::elf::link(llvm::makeArrayRef(arguments), llvm::outs(), llvm::errs(), true, false);
#else
	lld::elf::link(llvm::makeArrayRef(arguments), true, llvm::outs(), llvm::errs());
#endif

	return 0;
}
