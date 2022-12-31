#ifndef TESTHELPERS_H
#define TESTHELPERS_H

#include <llvm/IR/Verifier.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Config/llvm-config.h>
#if LLVM_VERSION_MAJOR >= 14
#include <llvm/MC/TargetRegistry.h>
#else
#include <llvm/Support/TargetRegistry.h>
#endif
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/IR/ValueSymbolTable.h>

#include <peglib.h>
#include <incbin.h>
#include <grammar.h>

#include "KaleidoscopeJIT.h"
#include "Visitor.h"
#include "EntryPoint.h"
#include "ModuleInfo.h"
#include "Visitor.h"
#include "CallableNode.h"
#include "Utility.h"

const CallableNode *compileFunction(std::string source)
{
	peg::parser parser(grammar());

	parser.enable_ast();
	parser.enable_packrat_parsing();

	std::shared_ptr<peg::Ast> ast;

	parser.parse(source, ast);

	Visitor visitor(std::vector<std::string>(), nullptr, nullptr, nullptr, nullptr);

	auto moduleInfo = visitor.visit(ast);

	for (auto &[_, function] : moduleInfo.functions)
	{
		return function;
	}

	return nullptr;
}

const BaseValue *compileValue(std::string source)
{
	std::stringstream stream;
	stream << "global dummy: ";
	stream << source;
	stream << ";";

	peg::parser parser(grammar());

	parser.enable_ast();
	parser.enable_packrat_parsing();

	std::shared_ptr<peg::Ast> ast;

	auto src = stream.str();

	parser.parse(src, ast);

	Visitor visitor(std::vector<std::string>(), nullptr, nullptr, nullptr, nullptr);

	auto moduleInfo = visitor.visit(ast);

	for (auto &[name, global] : moduleInfo.globals)
	{
		auto context = new llvm::LLVMContext();
		auto module = new llvm::Module("dz", *context);

		auto functionType = llvm::FunctionType::get(llvm::Type::getVoidTy(*context), false);
		auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, "dummy", module);

		auto alloc = llvm::BasicBlock::Create(*context, "alloc");
		auto block = llvm::BasicBlock::Create(*context, "block");

		linkBlocks(alloc, block);

		EntryPoint entryPoint(0
			, -1
			, nullptr
			, nullptr
			, block
			, alloc
			, function
			, module
			, context
			, "entry"
			, moduleInfo.functions
			, moduleInfo.locals
			, moduleInfo.globals
			, moduleInfo.types
			, moduleInfo.roots
			, Stack()
			, nullptr
			);

		Emitter emitter;

		for (auto &[_, values] : global->accept(emitter, { entryPoint, Stack() }))
		{
			return *values.begin();
		}
	}

	return nullptr;
}

EntryPoint compile(std::string source)
{
	peg::parser parser(grammar());

	parser.enable_ast();
	parser.enable_packrat_parsing();

	std::shared_ptr<peg::Ast> ast;

	parser.parse(source, ast);

	Visitor visitor(std::vector<std::string>(), nullptr, nullptr, nullptr, nullptr);

	auto moduleInfo = visitor
		.visit(ast);

	auto context = new llvm::LLVMContext();
	auto module = new llvm::Module("dz", *context);

	auto functionType = llvm::FunctionType::get(llvm::Type::getVoidTy(*context), false);
	auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, "dummy", module);

	auto alloc = llvm::BasicBlock::Create(*context, "alloc", function);
	auto block = llvm::BasicBlock::Create(*context, "block", function);

	return EntryPoint(0
		, -1
		, nullptr
		, nullptr
		, block
		, alloc
		, function
		, module
		, context
		, "entry"
		, moduleInfo.functions
		, moduleInfo.locals
		, moduleInfo.globals
		, moduleInfo.types
		, moduleInfo.roots
		, Stack()
		, nullptr
		);
}

int exec(std::string source)
{
	static peg::parser parser(grammar());

	parser.enable_ast();
	parser.enable_packrat_parsing();

	std::shared_ptr<peg::Ast> ast;

	parser.parse(source, ast);

	Visitor visitor(std::vector<std::string>(), nullptr, nullptr, nullptr, nullptr);

	auto llvmContext = std::make_unique<llvm::LLVMContext>();
	auto module = std::make_unique<llvm::Module>("dz", *llvmContext);

	auto moduleInfo = visitor
		.visit(ast);

	EntryPoint entryPoint(0
		, -1
		, nullptr
		, nullptr
		, nullptr
		, nullptr
		, nullptr
		, module.get()
		, llvmContext.get()
		, "entry"
		, moduleInfo.functions
		, moduleInfo.locals
		, moduleInfo.globals
		, moduleInfo.types
		, moduleInfo.roots
		, Stack()
		, nullptr
		);

	Emitter emitter;

	for (auto root : moduleInfo.roots)
	{
		root->accept(emitter, { entryPoint, Stack() });
	}

#ifdef DEBUG
	module->print(llvm::errs(), nullptr);
#endif

	if (verifyModule(*module, &llvm::errs()))
	{
		return -1;
	}

	auto jit = llvm::orc::KaleidoscopeJIT::Create();

	if (!jit)
	{
		return -1;
	}

	module->setDataLayout((*jit)->getDataLayout());

	auto threadSafeModule = llvm::orc::ThreadSafeModule(
		std::move(module),
		std::move(llvmContext)
		);

	auto error = (*jit)->addModule(std::move(threadSafeModule));

	if (error)
	{
		return -1;
	}

	auto mainSymbol = (*jit)->lookup("main");

	if (!mainSymbol)
	{
		return -1;
	}

	auto main = (int(*)())mainSymbol->getAddress();

	return main();
}

#endif // TESTHELPERS_H
