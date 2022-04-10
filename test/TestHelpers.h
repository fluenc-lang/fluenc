#ifndef TESTHELPERS_H
#define TESTHELPERS_H

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

#include "antlr4-runtime/fluencBaseVisitor.h"
#include "antlr4-runtime/fluencLexer.h"
#include "antlr4-runtime/fluencParser.h"

#include "KaleidoscopeJIT.h"
#include "Visitor.h"
#include "Utility.h"
#include "EntryPoint.h"

#include "nodes/CallableNode.h"
#include "nodes/GlobalNode.h"

CallableNode *compileFunction(std::string source)
{
	std::stringstream stream(source);

	antlr4::ANTLRInputStream input(stream);
	fluencLexer lexer(&input);
	antlr4::CommonTokenStream tokens(&lexer);
	fluencParser parser(&tokens);

	auto program = parser.program();

	Visitor visitor(nullptr, nullptr, nullptr);

	for (auto instruction : program->instruction())
	{
		return visitor.visit<CallableNode *>(instruction);
	}

	return nullptr;
}

const BaseValue *compileValue(std::string source)
{
	std::stringstream stream;
	stream << "global dummy: ";
	stream << source;
	stream << ";";

	antlr4::ANTLRInputStream input(stream);
	fluencLexer lexer(&input);
	antlr4::CommonTokenStream tokens(&lexer);
	fluencParser parser(&tokens);

	auto program = parser.program();

	Visitor visitor(nullptr, nullptr, nullptr);

	for (auto instruction : program->instruction())
	{
		auto global = visitor.visit<GlobalNode *>(instruction);

		auto context = std::make_unique<llvm::LLVMContext>();
		auto module = std::make_unique<llvm::Module>("dz", *context);

		auto functionType = llvm::FunctionType::get(llvm::Type::getVoidTy(*context), false);
		auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, "dummy", module.get());

		auto alloc = llvm::BasicBlock::Create(*context, "alloc", function);
		auto block = llvm::BasicBlock::Create(*context, "block", function);

		EntryPoint entryPoint(0
			, nullptr
			, nullptr
			, block
			, alloc
			, nullptr
			, nullptr
			, &module
			, &context
			, "entry"
			, std::multimap<std::string, CallableNode *>()
			, std::map<std::string, const BaseValue *>()
			, std::map<std::string, const Node *>()
			, std::map<std::string, Prototype *>()
			, Stack()
			, nullptr
			);

		for (auto &[_, values] : global->build(entryPoint, Stack()))
		{
			return *values.begin();
		}
	}

	return nullptr;
}

EntryPoint compile(std::string source)
{
	std::stringstream stream(source);

	antlr4::ANTLRInputStream input(stream);
	fluencLexer lexer(&input);
	antlr4::CommonTokenStream tokens(&lexer);
	fluencParser parser(&tokens);

	auto program = parser.program();

	Visitor visitor(nullptr, nullptr, nullptr);

	auto info = visitor
		.visit<ModuleInfo *>(program);

	auto entryPoint = info->entryPoint();

	auto &context = info->context();
	auto &module = info->module();

	auto functionType = llvm::FunctionType::get(llvm::Type::getVoidTy(*context), false);
	auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, "dummy", module.get());

	auto alloc = llvm::BasicBlock::Create(*context, "alloc", function);
	auto block = llvm::BasicBlock::Create(*context, "block", function);

	return EntryPoint(0
		, nullptr
		, nullptr
		, block
		, alloc
		, function
		, nullptr
		, &module
		, &context
		, "entry"
		, entryPoint.functions()
		, entryPoint.locals()
		, entryPoint.globals()
		, entryPoint.types()
		, Stack()
		, nullptr
		);
}

int exec(std::string source)
{
	std::stringstream stream(source);

	antlr4::ANTLRInputStream input(stream);
	fluencLexer lexer(&input);
	antlr4::CommonTokenStream tokens(&lexer);
	fluencParser parser(&tokens);

	auto program = parser.program();

	Visitor visitor(nullptr, nullptr, nullptr);

	auto moduleInfo = visitor
		.visit<ModuleInfo *>(program);

	auto threadSafeModule = llvm::orc::ThreadSafeModule(
		std::move(moduleInfo->module()),
		std::move(moduleInfo->context())
		);

	auto jit = llvm::orc::KaleidoscopeJIT::Create();

	if (!jit)
	{
		auto error = jit.takeError();

		return -1;
	}

	auto error = (*jit)->addModule(std::move(threadSafeModule));

	if (error)
	{
		return -1;
	}

	auto mainSymbol = (*jit)->lookup("main");

	auto main = (int(*)())mainSymbol->getAddress();

	return main();
}

#endif // TESTHELPERS_H
