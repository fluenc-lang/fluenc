#include <stdio.h>

#include <string_view>
#include <filesystem>
#include <fstream>

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

#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/FileSystemOptions.h>
#include <clang/Basic/LangOptions.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/CodeGen/CodeGenAction.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Lex/HeaderSearch.h>
#include <clang/Lex/HeaderSearchOptions.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Lex/PreprocessorOptions.h>
#include <clang/Parse/ParseAST.h>
#include <clang/Sema/Sema.h>

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
#include "exceptions/FileNotFoundException.h"

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

	peg::parser parser(grammar());

	parser.log = [](size_t line, size_t col, const std::string &msg)
	{
		std::cerr << line << ":" << col << ": " << msg << "\n";
	};

	parser.enable_ast();
	parser.enable_packrat_parsing();

	auto workingDirectory = std::filesystem::current_path();

	std::unordered_map<std::string, llvm::SmallVector<char>> objectFiles;
	std::vector<std::string> nativeSourceFiles;
	std::vector<std::string> foreignSourceFiles;

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

	struct CompilationResult
	{
		std::string name;
		std::unique_ptr<llvm::Module> module;
		std::unordered_map<std::string, std::string> files;
	};

	auto fluenc = [&](auto files) -> CompilationResult
	{
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
						objectFiles.insert({ path.string(), llvm::SmallVector<char>(buffer, buffer + size) });
					}
				}

				archive_read_free(archive);

				return module;
			});

			std::unordered_map<std::string, std::string> sourceFiles;

			auto module = std::accumulate(begin(files), end(files), baseModule, [&](auto accumulated, auto file)
			{
				std::ifstream stream(file);
				std::stringstream buffer;
				buffer << stream.rdbuf();

				auto [it, inserted] = sourceFiles.emplace(file, buffer.str());

				if (!inserted)
				{
					return accumulated;
				}

				auto module = analyze(it->first, it->second, parser);

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

				return {
					"native",
					std::move(llvmModule),
					sourceFiles
				};
			}

			return {
				"native",
				nullptr,
				sourceFiles
			};
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

	auto clang = [&](auto files) -> CompilationResult
	{
		clang::TextDiagnosticPrinter textDiagnosticPrinter(llvm::outs(), new clang::DiagnosticOptions);

		clang::CompilerInstance compiler;
		compiler.createDiagnostics(&textDiagnosticPrinter, false);

		auto &invocation = compiler.getInvocation();

		auto &headerSearchOptions = invocation.getHeaderSearchOpts();
		auto &targetOptions = invocation.getTargetOpts();
		auto &frontEndOptions = invocation.getFrontendOpts();

		targetOptions.Triple = llvm::sys::getDefaultTargetTriple();

		headerSearchOptions.AddPath("/usr/include", clang::frontend::IncludeDirGroup::Angled, false, false);
		headerSearchOptions.AddPath("/usr/lib/clang/14.0.6/include/", clang::frontend::IncludeDirGroup::Angled, false, false);

		std::transform(begin(files), end(files), std::back_inserter(frontEndOptions.Inputs), [](auto file)
		{
			return clang::FrontendInputFile(file, clang::Language::C);
		});

		auto action = std::make_unique<clang::EmitLLVMOnlyAction>(llvmContext.get());

		if (!compiler.ExecuteAction(*action))
		{
			return {};
		}

		auto module = action->takeModule();

		if (!module)
		{
			return {};
		}

		return {
			"foreign",
			std::move(module),
			std::unordered_map<std::string, std::string>()
		};
	};

	std::vector<CompilationResult> results;
	results.push_back(fluenc(nativeSourceFiles));
	results.push_back(clang(foreignSourceFiles));

	std::error_code error;

	for (auto &result : results)
	{
		if (!result.module)
		{
			continue;
		}

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

		passManager.run(*result.module);

		objectFiles.insert({ fmt::format("{}_{}.o", configuration->target, result.name), buffer });
	}

	if (configuration->type == "module")
	{
		auto moduleFileName = fmt::format("{}.fcm", configuration->target);

		auto archive = archive_write_new();

		archive_write_set_format_pax_restricted(archive);
		archive_write_open_filename(archive, moduleFileName.data());

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

		for (auto &result : results)
		{
			for (auto &[name, contents] : result.files)
			{
				addFile(name, contents);
			}
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
