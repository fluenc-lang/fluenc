#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/Optional.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

#include <sys/stat.h>

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <numeric>

#include "antlr4-runtime/dzBaseVisitor.h"
#include "antlr4-runtime/dzLexer.h"
#include "antlr4-runtime/dzParser.h"

#include "VisitorV1.h"
#include "DzValue.h"
#include "VisitorV2.h"

using namespace llvm;
using namespace antlr4;


int main()
{
	InitializeAllTargetInfos();
	InitializeAllTargets();
	InitializeAllTargetMCs();
	InitializeAllAsmParsers();
	InitializeAllAsmPrinters();

	std::ifstream stream;
	stream.open("main.dz");

	ANTLRInputStream input(stream);
	dzLexer lexer(&input);
	CommonTokenStream tokens(&lexer);
	dzParser parser(&tokens);

	auto program = parser.program();

	LLVMContext context;
	Module module("dz", context);

	VisitorV2 visitor(FunctionAttribute::None, nullptr, nullptr);

	visitor.visit(program);

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
