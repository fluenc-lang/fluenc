#include <stdio.h>

#include <string_view>
#include <filesystem>
#include <fstream>
#include <algorithm>

#include <archive.h>
#include <archive_entry.h>

#include <fmt/core.h>

#include <lld/Common/Driver.h>

#include <llvm/InitializePasses.h>

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Analysis/CFGPrinter.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Config/llvm-config.h>
#if LLVM_VERSION_MAJOR >= 14
#include <llvm/MC/TargetRegistry.h>
#else
#include <llvm/Support/TargetRegistry.h>
#endif
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Target/TargetMachine.h>

#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Driver/Driver.h>
#include <clang/Driver/Compilation.h>

#include "peglib.h"
#include "incbin.h"

#include <peglib.h>
#include <incbin.h>
#include <grammar.h>

#include "ProjectFileParser.h"
#include "CompilerException.h"
#include "Visitor.h"
#include "ModuleInfo.h"
#include "Emitter.h"
#include "CallableNode.h"

#include "exceptions/ParserException.h"

INCBIN_EXTERN(Grammar);

ModuleInfo analyze(const std::string &file, const std::string &source, peg::parser &parser)
{
	std::shared_ptr<peg::Ast> ast;

	parser.log = [&](size_t line, size_t col, const std::string &message)
	{
		throw new ParserException(file, line, col, message);
	};

	parser.parse(source, ast, file.c_str());

	Visitor visitor(std::vector<std::string>(), nullptr, nullptr, nullptr, nullptr);

	auto moduleInfo = visitor
		.visit(ast);

	return moduleInfo;
}

const char *getClangPath()
{
	auto fromEnv = std::getenv("CLANG_PATH");

	if (fromEnv)
	{
		return fromEnv;
	}

	return "clang";
}

llvm::ArrayRef<const char *> arrayRef(const std::vector<std::string> &input)
{
	std::vector<const char *> result;

	std::transform(begin(input), end(input), back_inserter(result), [](auto argument)
	{
		return strcpy(new char[argument.size() + 1], argument.c_str());
	});

	return llvm::makeArrayRef(result.data(), result.size());
}

int main(int argc, char **argv)
{
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

	auto llvmContext = std::make_unique<llvm::LLVMContext>();

	std::string errors;

	auto targetTriple = llvm::sys::getDefaultTargetTriple();
	auto target = llvm::TargetRegistry::lookupTarget(targetTriple, errors);

	std::cout << targetTriple << std::endl;

	if (!target)
	{
		llvm::errs() << errors;

		return 1;
	}

	auto clangPath = getClangPath();

	auto diagnostics = clang::CompilerInstance::createDiagnostics(new clang::DiagnosticOptions);

	clang::driver::Driver clangDriver(clangPath, targetTriple, *diagnostics, "fluenc");

	auto relocModel = llvm::Optional<llvm::Reloc::Model>();
	auto targetMachine = target->createTargetMachine(targetTriple, "generic", "", llvm::TargetOptions(), relocModel);
	auto dataLayout = targetMachine->createDataLayout();

	ProjectFileParser projectFileParser;

	auto configuration = projectFileParser.parse("project.toml");

	if (!configuration)
	{
		fmt::print("No project.toml file was found in the current directory\n");

		return 1;
	}

	peg::parser parser(grammar());

	parser.log = [](size_t line, size_t col, const std::string &msg)
	{
		std::cerr << line << ":" << col << ": " << msg << "\n";
	};

	parser.enable_ast();
	parser.enable_packrat_parsing();

	auto workingDirectory = std::filesystem::current_path();

	std::vector<std::filesystem::path> nativeSourceFiles;
	std::vector<std::filesystem::path> foreignSourceFiles;

	using iterator_t = std::filesystem::recursive_directory_iterator;

	for (auto i = iterator_t(workingDirectory); i != iterator_t(); i++)
	{
		auto relative = std::filesystem::relative(*i);

		if (relative.extension() == ".c")
		{
			foreignSourceFiles.push_back(relative.string());
		}
		else if (relative.extension() == ".fc")
		{
			nativeSourceFiles.push_back(relative.string());
		}
		else
		{
			fmt::print("Skipping file {}\n", relative.string());
		}
	}

	auto fluenc = [&](auto input) -> std::vector<std::filesystem::path>
	{
		std::vector<std::filesystem::path> output;

		try
		{
			auto baseModule = std::accumulate(begin(configuration->modules), end(configuration->modules), ModuleInfo {}, [&](auto accumulated, auto moduleName)
			{
				auto moduleFileName = fmt::format("{}.fcm", moduleName);

				auto archive = archive_read_new();

				archive_read_support_format_tar(archive);

				if (archive_read_open_filename(archive, moduleFileName.data(), 10240) != ARCHIVE_OK)
				{
					fmt::print("Failed to open module {} for reading: {}\n", moduleName, archive_error_string(archive));

					return accumulated;
				}

				archive_entry *entry = nullptr;

				auto module = accumulated;

				while (archive_read_next_header(archive, &entry) == ARCHIVE_OK)
				{
					auto size = archive_entry_size(entry);

					std::filesystem::path path(archive_entry_pathname(entry));

					auto buffer = new char[size];

					archive_read_data(archive, buffer, size);

					if (path.extension() == ".fc")
					{
						module = ModuleInfo::merge(module, analyze(path.string(), std::string(buffer, size), parser), true);
					}

					if (path.extension() == ".o")
					{
						std::ofstream stream(path, std::ios::out | std::ios::binary);

						stream.write(buffer, size);
						stream.close();

						output.push_back(path);
					}
				}

				archive_read_free(archive);

				return module;
			});

			auto module = std::accumulate(begin(input), end(input), baseModule, [&](auto accumulated, auto file)
			{
				std::ifstream stream(file);
				std::stringstream buffer;
				buffer << stream.rdbuf();

				output.push_back(file);

				auto module = analyze(file.string(), buffer.str(), parser);

				return ModuleInfo::merge(accumulated, module, false);
			});

			if (!module.roots.empty())
			{
				auto llvmModule = std::make_unique<llvm::Module>("native", *llvmContext);

				fmt::print("Building...\n");

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
					, module.roots
					, values
					, nullptr
					);

				Emitter emitter;

				for (auto &root : module.roots)
				{
					root->accept(emitter, { entryPoint, values });
				}

//				llvmModule->print(llvm::errs(), nullptr);
				llvmModule->setDataLayout(dataLayout);

				std::error_code error;

				auto objectFileName = fmt::format("{}_native.o", configuration->target);

				llvm::raw_fd_ostream destination(objectFileName, error);
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

				output.push_back(objectFileName);
			}

			return output;
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

			return {};
		}
	};

	auto clang = [&](auto input) -> std::vector<std::filesystem::path>
	{
		if (empty(input))
		{
			return input;
		}

		std::vector<std::filesystem::path> output;

		std::vector<std::string> compilerArguments = { clangPath, "-c" };

		for (auto file : input)
		{
			compilerArguments.push_back(file.string());

			output.push_back(file.stem() += ".o");
		}

		clang::SmallVector<std::pair<int, const clang::driver::Command *>> failingCommands;

		auto compilation = clangDriver.BuildCompilation(arrayRef(compilerArguments));

		for (auto &job : compilation->getJobs())
		{
			job.Print(llvm::errs(), "\n", true);
		}

		clangDriver.ExecuteCompilation(*compilation, failingCommands);

		return output;
	};

	auto fluencOutput = fluenc(nativeSourceFiles);
	auto clangOutput = clang(foreignSourceFiles);

	std::vector<std::filesystem::path> resultFiles;
	resultFiles.insert(end(resultFiles), begin(fluencOutput), end(fluencOutput));
	resultFiles.insert(end(resultFiles), begin(clangOutput), end(clangOutput));

	if (configuration->type == "module")
	{
		auto moduleFileName = fmt::format("{}.fcm", configuration->target);

		auto archive = archive_write_new();

		archive_write_set_format_pax_restricted(archive);
		archive_write_open_filename(archive, moduleFileName.data());

		for (auto &file : resultFiles)
		{
			auto size = std::filesystem::file_size(file);

			auto entry = archive_entry_new();

			archive_entry_set_pathname(entry, file.string().c_str());
			archive_entry_set_size(entry, size);
			archive_entry_set_filetype(entry, AE_IFREG);
			archive_entry_set_perm(entry, 0644);

			archive_write_header(archive, entry);

			std::ifstream stream(file, std::ios::in | std::ios::binary);

			for (char buffer[1 << 12]
				; stream.good()
				; archive_write_data(archive, buffer, stream.gcount()))
			{
				stream.read(buffer, sizeof(buffer));
			}

			archive_entry_free(entry);
		}

		archive_write_close(archive);
		archive_write_free(archive);
	}
	else
	{
		fmt::print("Linking {}...\n", configuration->target);

		std::vector<std::string> linkerArguments =
		{
			clangPath,
			"-o",
#ifdef _WIN32
			fmt::format("{}.exe", configuration->target)
#else
			"-no-pie",
			configuration->target.c_str()
#endif
		};

		for (auto &file : resultFiles)
		{
			if (file.extension() == ".o")
			{
				linkerArguments.push_back(file.string());
			}
		}

		for (auto &lib : configuration->libs)
		{
			linkerArguments.push_back(lib);
		}

		clang::SmallVector<std::pair<int, const clang::driver::Command *>> failingCommands;

		auto compilation = clangDriver.BuildCompilation(arrayRef(linkerArguments));

		for (auto &job : compilation->getJobs())
		{
			job.Print(llvm::errs(), "\n", false);
		}

		return clangDriver.ExecuteCompilation(*compilation, failingCommands);
	}

	return 0;
}
