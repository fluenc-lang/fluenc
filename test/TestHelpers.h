#ifndef TESTHELPERS_H
#define TESTHELPERS_H

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

#include "src/peglib.h"
#include "src/incbin.h"

#include "KaleidoscopeJIT.h"
#include "Visitor.h"
#include "EntryPoint.h"
#include "ModuleInfo.h"
#include "Visitor.h"

#include "nodes/CallableNode.h"

INCTXT(Grammar, "fluenc.peg");

CallableNode *compileFunction(std::string source)
{
	peg::parser parser(gGrammarData);

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

	peg::parser parser(gGrammarData);

	parser.enable_ast();
	parser.enable_packrat_parsing();

	std::shared_ptr<peg::Ast> ast;

	parser.parse(stream.str(), ast);

	Visitor visitor(std::vector<std::string>(), nullptr, nullptr, nullptr, nullptr);

	auto moduleInfo = visitor.visit(ast);

	for (auto &[name, global] : moduleInfo.globals)
	{
		auto context = new llvm::LLVMContext();
		auto module = new llvm::Module("dz", *context);

		auto functionType = llvm::FunctionType::get(llvm::Type::getVoidTy(*context), false);
		auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, "dummy", module);

		auto alloc = llvm::BasicBlock::Create(*context, "alloc", function);
		auto block = llvm::BasicBlock::Create(*context, "block", function);

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
			, Stack<BaseValue>()
			, nullptr
			);

		Emitter emitter;

		for (auto &[_, values] : global->accept(emitter, entryPoint, Stack<BaseValue>()))
		{
			return *values.begin();
		}
	}

	return nullptr;
}

EntryPoint compile(std::string source)
{
	peg::parser parser(gGrammarData);

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
		, Stack<BaseValue>()
		, nullptr
		);
}

int exec(std::string source)
{
	static peg::parser parser(gGrammarData);

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
		, Stack<BaseValue>()
		, nullptr
		);

	Emitter emitter;

	for (auto root : moduleInfo.roots)
	{
		root->accept(emitter, entryPoint, Stack<BaseValue>());
	}

	module->print(llvm::errs(), nullptr);

	auto threadSafeModule = llvm::orc::ThreadSafeModule(
		std::move(module),
		std::move(llvmContext)
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
