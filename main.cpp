#include <stdio.h>

#include <llvm/Support/TargetSelect.h>

#include "Tests.h"

#include <QTest>

extern "C" int putz(char *str)
{
	return puts(str);
}

int main(int argc, char **argv)
{
	llvm::InitializeAllTargetInfos();
	llvm::InitializeAllTargets();
	llvm::InitializeAllTargetMCs();
	llvm::InitializeAllAsmParsers();
	llvm::InitializeAllAsmPrinters();

	if (argc > 1)
	{
		std::string_view inputFileName(*(argv + 1));

		std::ostringstream outputFileName;
		outputFileName << inputFileName.substr(0, inputFileName.size() - 3);
		outputFileName << ".o";

		std::ifstream stream;
		stream.open(inputFileName.data());

		antlr4::ANTLRInputStream input(stream);
		dzLexer lexer(&input);
		antlr4::CommonTokenStream tokens(&lexer);
		dzParser parser(&tokens);

		auto program = parser.program();

		VisitorV4 visitor(nullptr);

		auto moduleInfo = visitor
			.visit(program)
			.as<ModuleInfo *>();

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

		moduleInfo->module()->setDataLayout(targetMachine->createDataLayout());

		std::error_code EC;
		llvm::raw_fd_ostream dest(outputFileName.str(), EC, llvm::sys::fs::OF_None);

		llvm::legacy::PassManager pm;

		targetMachine->addPassesToEmitFile(pm, dest, nullptr, llvm::CGFT_ObjectFile);

		pm.run(*moduleInfo->module());

		dest.flush();

		return 0;
	}

	Tests tests;

	QTest::qExec(&tests);

//	try
//	{

//	}
//	catch (CompilerException *exception)
//	{
//		std::cout << "main.dz:" << exception->row() << ":" << exception->column() << ": error: " << exception->message() << std::endl;
//		std::cout << std::setw(5) << exception->row() << " | ";

//		stream.seekg(0);

//		std::string line;

//		for (int i = 0; i < exception->row(); i++)
//		{
//			std::getline(stream, line);
//		}

//		auto trimmed = std::find_if(begin(line), end(line), [](unsigned char ch)
//		{
//			return !std::isspace(ch);
//		});

//		for (auto i = trimmed; i != end(line); i++)
//		{
//			std::cout << *i;
//		}

//		std::cout << std::endl;
//		std::cout << std::setw(8) << "| ";

//		for (auto i = trimmed; i != end(line); i++)
//		{
//			auto tokenSelector = [&]
//			{
//				if (i == begin(line) + exception->column())
//				{
//					return "^";
//				}

//				if (i < begin(line) + exception->column())
//				{
//					return " ";
//				}

//				if (i > begin(line) + exception->column() + exception->length())
//				{
//					return " ";
//				}

//				return "~";
//			};

//			std::cout << tokenSelector();
//		}

//		std::cout << std::endl;

//		return 1;
//	}

//	module.print(llvm::errs(), nullptr);

//	std::string error;

//	auto targetTriple = sys::getDefaultTargetTriple();
//	auto target = TargetRegistry::lookupTarget(targetTriple, error);

//	if (!target)
//	{
//		errs() << error;

//		return 1;
//	}

//	auto relocModel = Optional<Reloc::Model>();
//	auto targetMachine = target->createTargetMachine(targetTriple, "generic", "", TargetOptions(), relocModel);

//	module.setDataLayout(targetMachine->createDataLayout());

//	std::error_code EC;
//	raw_fd_ostream dest("output.o", EC, sys::fs::OF_None);

//	legacy::PassManager pm;

//	targetMachine->addPassesToEmitFile(pm, dest, nullptr, CGFT_ObjectFile);

//	pm.run(module);

//	dest.flush();

	return 0;

}
