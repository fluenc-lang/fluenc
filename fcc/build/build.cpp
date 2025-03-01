#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string_view>

#include <archive.h>
#include <archive_entry.h>

#include <fmt/core.h>

#include <llvm/Analysis/CFGPrinter.h>
#include <llvm/Config/llvm-config.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>
#if LLVM_VERSION_MAJOR >= 14
#include <llvm/MC/TargetRegistry.h>
#else
#include <llvm/Support/TargetRegistry.h>
#endif
#include <llvm/Support/FileSystem.h>
#if LLVM_VERSION_MAJOR >= 19
#include <llvm/TargetParser/Host.h>
#else
#include <llvm/Support/Host.h>
#endif
#include <llvm/Target/TargetOptions.h>

#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Driver/Compilation.h>
#include <clang/Frontend/CompilerInstance.h>

#include <grammar.h>
#include <incbin.h>
#include <peglib.h>

#include "ProjectFileParser.h"
#include "ast_transformer.hpp"
#include "emitter.hpp"

#include "ast/module_node.hpp"

#include "exceptions/compiler_exception.hpp"
#include "exceptions/parser_exception.hpp"

#include "build.hpp"
#include "dependencies.hpp"

INCBIN_EXTERN(Grammar);

std::string get_clang_path(std::filesystem::path exe)
{
	if (auto from_env = std::getenv("CLANG_PATH"))
	{
		return from_env;
	}

	auto clangExe = exe.parent_path() / "clang";

	return clangExe.string();
}

fluenc::module_node analyze(const std::string& file, const std::string& source, peg::parser& parser)
{
	std::shared_ptr<peg::Ast> ast;

	parser.set_logger([&](size_t line, size_t col, const std::string& message) {
		throw parser_exception(file, line, col, message);
	});

	parser.parse(source, ast, file.c_str());

	fluenc::ast_transformer visitor(std::vector<std::string>(), {}, {}, {}, {});

	return visitor.transform(ast);
}

llvm::ArrayRef<const char*> arrayRef(const std::vector<std::string>& input)
{
	auto result = new const char*[input.size()];

	std::transform(begin(input), end(input), result, [](auto argument) {
		return strcpy(new char[argument.size() + 1], argument.c_str());
	});

	return { result, input.size() };
}

bool build(const BuildContext& context)
{
	auto dataLayout = context.targetMachine->createDataLayout();

	ProjectFileParser project_file_parser;

	auto configuration = project_file_parser.parse(context.environment.source / "project.toml");

	if (!configuration)
	{
		fmt::print("No project.toml file was found in the current directory\n");

		return 1;
	}

	if (!build_dependencies(*configuration, context))
	{
		return 1;
	}

	peg::parser parser(grammar());

	parser.set_logger([](size_t line, size_t col, const std::string& msg) {
		std::cerr << line << ":" << col << ": " << msg << "\n";
	});

	parser.enable_ast();
	parser.enable_packrat_parsing();

	std::vector<std::filesystem::path> native_source_files;
	std::vector<std::filesystem::path> foreign_source_files;

	using iterator_t = std::filesystem::recursive_directory_iterator;

	for (auto i = iterator_t(context.environment.source); i != iterator_t(); i++)
	{
		auto relative = std::filesystem::relative(*i, context.environment.source);

		if (*relative.string().begin() == '.')
		{
			i.disable_recursion_pending();

			continue;
		}

		if (relative.extension() == ".c")
		{
			foreign_source_files.push_back(relative.string());
		}
		else if (relative.extension() == ".fc")
		{
			native_source_files.push_back(relative.string());
		}
		else if (context.options.verbose)
		{
			fmt::print("Skipping file {}\n", relative.string());
		}
	}

	auto fluenc = [&](auto input) -> std::vector<std::filesystem::path> {
		std::vector<std::filesystem::path> output;

		try
		{
			auto base_module = std::accumulate(
				begin(configuration->modules),
				end(configuration->modules),
				fluenc::module_node {},
				[&](auto accumulated, auto moduleName) {
					auto module_file_name = context.environment.root / ".fluenc" / "modules" /
											fmt::format("{}.fcm", moduleName);

					auto module_file_name_string = module_file_name.string();

					auto archive = archive_read_new();

					archive_read_support_format_tar(archive);

					if (archive_read_open_filename(archive, module_file_name_string.c_str(), 10240) != ARCHIVE_OK)
					{
						fmt::print("Failed to open module {} for reading: {}\n", moduleName, archive_error_string(archive));

						return accumulated;
					}

					archive_entry* entry = nullptr;

					auto module = accumulated;

					while (archive_read_next_header(archive, &entry) == ARCHIVE_OK)
					{
						auto size = archive_entry_size(entry);

						std::filesystem::path path(archive_entry_pathname(entry));

						auto buffer = new char[size];

						archive_read_data(archive, buffer, size);

						if (path.extension() == ".fc")
						{
							module = fluenc::module_node::
								merge(module, analyze(path.string(), std::string(buffer, size), parser), true);
						}

						if (path.extension() == ".o")
						{
							std::ofstream stream(context.environment.source / path, std::ios::out | std::ios::binary);

							stream.write(buffer, size);
							stream.close();

							output.push_back(path);
						}
					}

					archive_read_free(archive);

					return module;
				}
			);

			auto module = std::accumulate(begin(input), end(input), base_module, [&](auto accumulated, auto file) {
				std::ifstream stream(context.environment.source / file);
				std::stringstream buffer;
				buffer << stream.rdbuf();

				output.push_back(file);

				auto module = analyze(file.string(), buffer.str(), parser);

				return fluenc::module_node::merge(accumulated, module, false);
			});

			if (!module.roots.empty())
			{
				auto llvm_module = std::make_unique<llvm::Module>("native", *context.llvmContext);

				fmt::print("Building '{}'...\n", configuration->target);

				fluenc::code_generation::value_stack values;

				fluenc::code_generation::entry_point entryPoint(
					0,
					-1,
					nullptr,
					nullptr,
					nullptr,
					nullptr,
					nullptr,
					llvm_module.get(),
					context.llvmContext,
					"term",
					module.functions,
					module.locals,
					module.globals,
					module.types,
					module.roots,
					values,
					nullptr
				);

				fluenc::code_generation::emitter emitter({});

				for (auto& root : module.roots)
				{
					emitter.visit(root, { entryPoint, values });
				}

				//				llvmModule->print(llvm::errs(), nullptr);
				llvm_module->setDataLayout(dataLayout);

				std::error_code error;

				auto object_file_name = context.environment.source / fmt::format("{}_native.o", configuration->target);

				auto object_file_name_string = object_file_name.string();

				llvm::raw_fd_ostream destination(object_file_name_string.c_str(), error);
				llvm::legacy::PassManager pass_manager;

#if LLVM_VERSION_MAJOR < 19
				if (context.options.generateDotCfg)
				{
					pass_manager.add(llvm::createCFGPrinterLegacyPassPass());
				}

				context.targetMachine->addPassesToEmitFile(pass_manager, destination, nullptr, llvm::CGFT_ObjectFile);
#else
				context.targetMachine
					->addPassesToEmitFile(pass_manager, destination, nullptr, llvm::CodeGenFileType::ObjectFile);
#endif

				pass_manager.run(*llvm_module);

				output.push_back(object_file_name.filename());
			}

			return output;
		}
		catch (const compiler_exception& exception)
		{
			std::cout << exception.file() << ":" << exception.row() << ":" << exception.column()
					  << ": error: " << exception.message() << std::endl;
			std::cout << std::setw(5) << exception.row() << " | ";

			std::ifstream stream;

			stream.open(exception.file());

			std::string line;

			for (size_t i = 0u; i < exception.row(); i++)
			{
				std::getline(stream, line);
			}

			auto trimmed = std::find_if(begin(line), end(line), [](unsigned char ch) {
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
				auto tokenSelector = [&] {
					if (i == begin(line) + exception.column())
					{
						return "^";
					}

					if (i < begin(line) + exception.column())
					{
						return " ";
					}

					if (i > begin(line) + exception.column() + exception.length())
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

	auto with_extension = [](std::filesystem::path path, const std::filesystem::path& new_extension) {
		path.replace_extension(new_extension);

		return path;
	};

	auto clang = [&](auto input) -> std::vector<std::filesystem::path> {
		std::vector<std::filesystem::path> output;

		std::transform(begin(input), end(input), back_inserter(output), [&](auto file) {
			fmt::print("Building '{}'...\n", file.string());

			auto input_file = context.environment.source / file;
			auto output_file = with_extension(input_file, ".o");

			std::vector<std::string> compiler_arguments = { "fcc", "-c", input_file.string(), "-o", output_file.string() };

			clang::SmallVector<std::pair<int, const clang::driver::Command*>> failing_commands;

			auto compilation = context.clangDriver->BuildCompilation(arrayRef(compiler_arguments));

			if (context.options.verbose)
			{
				for (auto& job : compilation->getJobs())
				{
					job.Print(llvm::errs(), "\n", false);
				}
			}

			context.clangDriver->ExecuteCompilation(*compilation, failing_commands);

			return output_file;
		});

		return output;
	};

	auto fluenc_output = fluenc(native_source_files);
	auto clang_output = clang(foreign_source_files);

	std::vector<std::filesystem::path> result_files;
	result_files.insert(end(result_files), begin(fluenc_output), end(fluenc_output));
	result_files.insert(end(result_files), begin(clang_output), end(clang_output));

	if (configuration->type == "module")
	{
		auto module_file_name = context.environment.target / fmt::format("{}.fcm", configuration->target);

		auto module_file_name_string = module_file_name.string();

		auto archive = archive_write_new();

		archive_write_set_format_pax_restricted(archive);
		archive_write_open_filename(archive, module_file_name_string.c_str());

		for (auto& file : result_files)
		{
			auto size = std::filesystem::file_size(context.environment.source / file);

			auto entry = archive_entry_new();

			auto file_string = file.string();

			archive_entry_set_pathname(entry, file_string.c_str());
			archive_entry_set_size(entry, size);
			archive_entry_set_filetype(entry, AE_IFREG);
			archive_entry_set_perm(entry, 0644);

			archive_write_header(archive, entry);

			std::ifstream stream(context.environment.source / file, std::ios::in | std::ios::binary);

			for (char buffer[1 << 12]; stream.good(); archive_write_data(archive, buffer, stream.gcount()))
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
		auto target = context.environment.target / configuration->target;

		fmt::print("Linking '{}'...\n", configuration->target);

		std::vector<std::string> linker_arguments = { "fcc",
													  "-o",
#ifdef _WIN32
													  fmt::format("{}.exe", target.string())
#else
			target.string(),
			"-no-pie",
#endif
		};

		for (auto& file : result_files)
		{
			if (file.extension() == ".o")
			{
				linker_arguments.push_back(file.string());
			}
		}

		for (auto& lib : configuration->libs)
		{
			linker_arguments.push_back(lib);
		}

		clang::SmallVector<std::pair<int, const clang::driver::Command*>> failingCommands;

		auto compilation = context.clangDriver->BuildCompilation(arrayRef(linker_arguments));

		if (context.options.verbose)
		{
			for (auto& job : compilation->getJobs())
			{
				job.Print(llvm::errs(), "\n", false);
			}
		}

		context.clangDriver->ExecuteCompilation(*compilation, failingCommands);

		if (!failingCommands.empty())
		{
			return false;
		}
	}

	return true;
}

int bootstrap_build(char** argv, const BuildEnvironment& environment, const BuildOptions& options)
{
	auto llvm_context = std::make_unique<llvm::LLVMContext>();

	std::string errors;

	auto target_triple = llvm::sys::getDefaultTargetTriple();
	auto target = llvm::TargetRegistry::lookupTarget(target_triple, errors);

	if (options.verbose)
	{
		fmt::print("Building for target '{}'\n", target_triple);
	}

	if (!target)
	{
		llvm::errs() << errors;

		return 1;
	}

	auto clang_path = get_clang_path(*argv);

	auto diagnostics = clang::CompilerInstance::createDiagnostics(new clang::DiagnosticOptions);

	clang::driver::Driver clang_driver(clang_path, target_triple, *diagnostics, "fluenc");

	auto target_machine = target->createTargetMachine(target_triple, "generic", "", llvm::TargetOptions(), {});

	BuildContext context = {
		.options = options,
		.environment = environment,
		.clangDriver = &clang_driver,
		.targetMachine = target_machine,
		.llvmContext = llvm_context.get(),
	};

	if (!build(context))
	{
		return 1;
	}

	return 0;
}
