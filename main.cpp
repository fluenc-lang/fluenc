#include <llvm/Support/TargetSelect.h>

#include "Tests.h"

#include <QTest>

int main()
{
	llvm::InitializeAllTargetInfos();
	llvm::InitializeAllTargets();
	llvm::InitializeAllTargetMCs();
	llvm::InitializeAllAsmParsers();
	llvm::InitializeAllAsmPrinters();

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
