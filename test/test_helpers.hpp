#pragma once

#include <llvm/Config/llvm-config.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#if LLVM_VERSION_MAJOR >= 14
#include <llvm/MC/TargetRegistry.h>
#else
#include <llvm/Support/TargetRegistry.h>
#endif
#include <llvm/IR/ValueSymbolTable.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

#include <fmt/core.h>
#include <grammar.h>
#include <incbin.h>
#include <peglib.h>

#include "KaleidoscopeJIT.h"
#include "ast.hpp"
#include "ast_transformer.hpp"
#include "emitter.hpp"
#include "entry_point.hpp"
#include "utility.hpp"

#include "ast/function_node.hpp"
#include "ast/module_node.hpp"

const fluenc::function_node* compile_function(std::string source)
{
	peg::parser parser(grammar());

	parser.enable_ast();
	parser.enable_packrat_parsing();

	std::shared_ptr<peg::Ast> ast;

	parser.parse(source, ast);

	fluenc::ast_transformer transformer(std::vector<std::string>(), {}, {}, {}, {});

	auto module_info = transformer.transform(ast);

	for (auto& [_, function] : module_info.functions)
	{
		return function;
	}

	return nullptr;
}

const fluenc::code_generation::base_value* compile_value(std::string source)
{
	auto global = fmt::format("global dummy: {};", source);

	peg::parser parser(grammar());

	parser.enable_ast();
	parser.enable_packrat_parsing();

	std::shared_ptr<peg::Ast> ast;

	parser.parse(global, ast);

	fluenc::ast_transformer transformer(std::vector<std::string>(), {}, {}, {}, {});

	auto module_info = transformer.transform(ast);

	for (auto& [name, global] : module_info.globals)
	{
		auto context = new llvm::LLVMContext();
		auto module = new llvm::Module("dz", *context);

		auto function_type = llvm::FunctionType::get(llvm::Type::getVoidTy(*context), false);
		auto function = llvm::Function::Create(function_type, llvm::Function::ExternalLinkage, "dummy", module);

		auto alloc = llvm::BasicBlock::Create(*context, "alloc");
		auto block = llvm::BasicBlock::Create(*context, "block");

		link_blocks(alloc, block);

		fluenc::code_generation::entry_point entry_point(
			0,
			-1,
			nullptr,
			nullptr,
			block,
			alloc,
			function,
			module,
			context,
			"entry",
			module_info.functions,
			module_info.locals,
			module_info.globals,
			module_info.types,
			module_info.roots,
			fluenc::code_generation::value_stack(),
			nullptr
		);

		fluenc::code_generation::emitter emitter({});

		for (auto& [_, values] :
			 fluenc::accept(global, emitter, { entry_point, fluenc::code_generation::value_stack() }))
		{
			return *values.begin();
		}
	}

	return nullptr;
}

fluenc::code_generation::entry_point compile(std::string source)
{
	peg::parser parser(grammar());

	parser.enable_ast();
	parser.enable_packrat_parsing();

	std::shared_ptr<peg::Ast> ast;

	parser.parse(source, ast);

	fluenc::ast_transformer transformer(std::vector<std::string>(), {}, {}, {}, {});

	auto module_info = transformer.transform(ast);

	auto context = new llvm::LLVMContext();
	auto module = new llvm::Module("dz", *context);

	auto function_type = llvm::FunctionType::get(llvm::Type::getVoidTy(*context), false);
	auto function = llvm::Function::Create(function_type, llvm::Function::ExternalLinkage, "dummy", module);

	auto alloc = llvm::BasicBlock::Create(*context, "alloc", function);
	auto block = llvm::BasicBlock::Create(*context, "block", function);

	return fluenc::code_generation::entry_point(
		0,
		-1,
		nullptr,
		nullptr,
		block,
		alloc,
		function,
		module,
		context,
		"entry",
		module_info.functions,
		module_info.locals,
		module_info.globals,
		module_info.types,
		module_info.roots,
		fluenc::code_generation::value_stack(),
		nullptr
	);
}

int exec(std::string source)
{
	static peg::parser parser(grammar());

	parser.enable_ast();
	parser.enable_packrat_parsing();

	std::shared_ptr<peg::Ast> ast;

	parser.parse(source, ast);

	fluenc::ast_transformer transformer(std::vector<std::string>(), {}, {}, {}, {});

	auto llvm_context = std::make_unique<llvm::LLVMContext>();
	auto module = std::make_unique<llvm::Module>("dz", *llvm_context);

	auto module_info = transformer.transform(ast);

	fluenc::code_generation::entry_point entryPoint(
		0,
		-1,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		module.get(),
		llvm_context.get(),
		"entry",
		module_info.functions,
		module_info.locals,
		module_info.globals,
		module_info.types,
		module_info.roots,
		fluenc::code_generation::value_stack(),
		nullptr
	);

	fluenc::code_generation::emitter emitter({});

	for (auto root : module_info.roots)
	{
		emitter.visit(root, { entryPoint, fluenc::code_generation::value_stack() });
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

	auto thread_safe_module = llvm::orc::ThreadSafeModule(std::move(module), std::move(llvm_context));

	auto error = (*jit)->addModule(std::move(thread_safe_module));

	if (error)
	{
		return -1;
	}

	auto main_symbol = (*jit)->lookup("main");

	if (!main_symbol)
	{
		return -1;
	}

	auto main = main_symbol->getAddress();

#if LLVM_VERSION_MAJOR >= 17
	return main.toPtr<int (*)()>()();
#else
	return reinterpret_cast<int (*)()>(main)();
#endif
}
