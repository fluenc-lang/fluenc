#include <stdio.h>

#include <string_view>
#include <filesystem>
#include <fstream>

#include <archive.h>
#include <archive_entry.h>

#include <fmt/core.h>

#include <lld/Common/Driver.h>

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

#include <llvm/Support/Host.h>
#include <llvm/Target/TargetMachine.h>

#include "peglib.h"
#include "incbin.h"

#include "ProjectFileParser.h"
#include "CompilerException.h"
#include "Visitor.h"
#include "ModuleInfo.h"
#include "Emitter.h"
#include "CallableNode.h"

#include "exceptions/ParserException.h"
#include "exceptions/FileNotFoundException.h"

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
	fmt::print("{} depends on {}\n", job.sourceFile, current.sourceFile);

	auto [_, success] = processed.insert(current.sourceFile);

	if (!success)
	{
		return false;
	}

	if (current.sourceFileTime > job.objectFileTime)
	{
		fmt::print("{} is more recent than {}\n", current.sourceFile, job.sourceFile);

		return true;
	}

	for (auto &use : current.module.uses)
	{
		auto translated = translateUse(use.first, current);
		auto childJob = jobs.find(translated);

		if (childJob == jobs.end())
		{
			throw new FileNotFoundException(use.second);
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
		fmt::print("No output found for {}\n", job.sourceFile);

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
	fmt::print("Adding info from {}\n", job.sourceFile);

	auto [_, success] = processed.insert(job.sourceFile);

	if (!success)
	{
		return ModuleInfo();
	}

	auto target = job.module;

	for (auto &use : job.module.uses)
	{
		auto translated = translateUse(use.first, job);
		auto childJob = jobs.find(translated);

		if (childJob == jobs.end())
		{
			throw new FileNotFoundException(use.second);
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

	if (!configuration)
	{
		fmt::print("No project.toml file was found in the current directory\n");

		return 1;
	}

	peg::parser parser(gGrammarData);

	parser.log = [](size_t line, size_t col, const std::string &msg)
	{
		std::cerr << line << ":" << col << ": " << msg << "\n";
	};

	parser.enable_ast();
	parser.enable_packrat_parsing();

	std::unordered_map<std::string, CompilerJob> jobs;

	auto workingDirectory = std::filesystem::current_path();

	std::unordered_map<std::string, llvm::SmallVector<char>> objectFiles;
	std::unordered_map<std::string, std::string> sourceFiles;

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
				module = ModuleInfo::merge(module, analyze(path.string(), std::string(buffer, size), parser));
			}

			if (path.extension() == ".o")
			{
				objectFiles.insert({ path.string(), llvm::SmallVector<char>(buffer, buffer + size) });
			}
		}

		archive_read_free(archive);

		return module;
	});

	try
	{
		using iterator_t = std::filesystem::recursive_directory_iterator;

		auto module = std::accumulate(iterator_t(workingDirectory), iterator_t(), baseModule, [&](auto accumulated, auto entry)
		{
			auto path = entry.path();

			if (path.extension() != ".fc")
			{
				fmt::print("Skipping file {}\n", path.string());

				return accumulated;
			}

			fmt::print("Adding file {}\n", path.string());

			auto relative = std::filesystem::relative(path);

			std::ifstream stream(relative);
			std::stringstream buffer;
			buffer << stream.rdbuf();

			auto [it, inserted] = sourceFiles.emplace(relative.string(), buffer.str());

			if (!inserted)
			{
				return accumulated;
			}

			auto module = analyze(it->first, it->second, parser);

			return ModuleInfo::merge(accumulated, module);
		});

		auto llvmModule = std::make_unique<llvm::Module>("main", *llvmContext);

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

		llvmModule->print(llvm::errs(), nullptr);
		llvmModule->setDataLayout(dataLayout);

		std::error_code error;

		llvm::SmallVector<char> buffer;

		llvm::raw_svector_ostream destination(buffer);
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

		objectFiles.insert({ "module.o", buffer });
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

		return 1;
	}

	if (configuration->type == "module")
	{
		auto archive = archive_write_new();

		archive_write_set_format_pax_restricted(archive);
		archive_write_open_filename(archive, "module.fcm");

		auto addFile = [=](auto name, auto contents)
		{
			auto entry = archive_entry_new();

			archive_entry_set_pathname(entry, name.data());
			archive_entry_set_size(entry, contents.size());
			archive_entry_set_filetype(entry, AE_IFREG);
			archive_entry_set_perm(entry, 0644);

			archive_write_header(archive, entry);
			archive_write_data(archive, contents.data(), contents.size());

			archive_entry_free(entry);
		};

		for (auto &[name, contents] : objectFiles)
		{
			addFile(name, contents);
		}

		for (auto &[name, contents] : sourceFiles)
		{
			addFile(name, contents);
		}

		archive_write_close(archive);
		archive_write_free(archive);
	}
	else
	{
		fmt::print("Linking {}...\n", configuration->target);

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

		std::error_code error;

		for (auto &[name, contents] : objectFiles)
		{
			linkerArguments.push_back(name);

			llvm::raw_fd_ostream destination(name, error, llvm::sys::fs::OF_None);

			for (auto i = 0u; i < contents.size(); i += 1024)
			{
				destination.write(contents.data() + i
					, std::min(1024ul, contents.size() - i)
					);
			}

			destination.flush();
		}

		linkerArguments.insert(end(linkerArguments), begin(configuration->libs), end(configuration->libs));

		linkerArguments.push_back("-o");
		linkerArguments.push_back(configuration->target);

		std::vector<const char *> arguments;

		std::transform(begin(linkerArguments), end(linkerArguments), std::back_inserter(arguments), [](auto argument)
		{
			return strcpy(new char[argument.size() + 1], argument.c_str());
		});

#if LLVM_VERSION_MAJOR >= 14
		lld::elf::link(llvm::makeArrayRef(arguments), llvm::outs(), llvm::errs(), true, false);
#else
		lld::elf::link(llvm::makeArrayRef(arguments), true, llvm::outs(), llvm::errs());
#endif
	}

	return 0;
}
