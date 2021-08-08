#include "VisitorV3.h"
#include "FunctionAttribute.h"

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
#include <llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/RTDyldMemoryManager.h>
#include <llvm/ExecutionEngine/RuntimeDyld.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/ObjectLinkingLayer.h>
#include <llvm/ExecutionEngine/JITSymbol.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/Mangler.h>
#include <llvm/Support/DynamicLibrary.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetSelect.h>

#include "DzBinaryNg.h"
#include "DzCallNg.h"
#include "DzConstantNg.h"
#include "DzEntryPointNg.h"
#include "DzFunctionNg.h"
#include "DzMemberAccessNg.h"
#include "DzMemberNg.h"
#include "DzNodeNg.h"
#include "DzReturnNg.h"
#include "DzTypeNameNg.h"
#include "EntryPointInfo.h"

class DzTypeNameNg;
class DzCallable;
class DzValueNg;

antlrcpp::Any VisitorV3::visitProgram(dzParser::ProgramContext *context)
{
	std::map<std::string, DzCallable *> functions;
	std::vector<DzCallable *> roots;

	for (auto function : context->function())
	{
		auto q = visit(function);

		if (q.is<DzEntryPointNg *>())
		{
			auto ep = q.as<DzEntryPointNg *>();

			roots.push_back(ep);
		}

		if (q.is<DzFunctionNg *>())
		{
			auto fn = q.as<DzFunctionNg *>();

			functions[fn->name()] = fn;
		}
	}

	auto llvmContext = std::make_unique<llvm::LLVMContext>();
	auto module = std::make_unique<llvm::Module>("dz", *llvmContext);

	for (auto root : roots)
	{
		std::map<std::string, llvm::Value *> locals;

		EntryPointInfo entryPoint(root->returnType(), llvmContext, module, nullptr, nullptr, functions, locals, "");

		root->build(entryPoint);
	}

	module->print(llvm::errs(), nullptr);

	return new ModuleInfo(std::move(module), std::move(llvmContext));

//	std::string error;

//	auto targetTriple = llvm::sys::getDefaultTargetTriple();
//	auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);

//	if (!target)
//	{
//		llvm::errs() << error;

//		return 1;
//	}

//	auto relocModel = llvm::Optional<llvm::Reloc::Model>();
//	auto targetMachine = target->createTargetMachine(targetTriple, "generic", "", llvm::TargetOptions(), relocModel);

//	module.setDataLayout(targetMachine->createDataLayout());

//	std::error_code EC;
//	llvm::raw_fd_ostream dest("output.o", EC, llvm::sys::fs::OF_None);

//	llvm::legacy::PassManager pm;

//	targetMachine->addPassesToEmitFile(pm, dest, nullptr, llvm::CGFT_ObjectFile);

//	pm.run(module);

//	dest.flush();
}

FunctionAttribute getAttribute2(dzParser::FunctionContext *ctx)
{
	if (ctx->attribute)
	{
		auto attribute = ctx->attribute->getText();

		if (attribute == "import")
		{
			return FunctionAttribute::Import;
		}

		if (attribute == "export")
		{
			return FunctionAttribute::Export;
		}
	}

	return FunctionAttribute::None;
}

antlrcpp::Any VisitorV3::visitFunction(dzParser::FunctionContext *context)
{
	auto attribute = getAttribute2(context);

	if (attribute == FunctionAttribute::Import)
	{
		return defaultResult();
	}

	auto block = context->block();

	if (block)
	{
		auto name = context->name->getText();

		auto node = visit(block)
			.as<DzNodeNg>();

		std::vector<DzMemberNg *> arguments;

		for (auto argument : context->argument())
		{
			auto argumentType = visit(argument)
				.as<DzMemberNg *>();

			arguments.push_back(argumentType);
		}

		if (attribute == FunctionAttribute::Export)
		{
			auto returnType = visit(context->typeName())
				.as<DzTypeNameNg *>();

			return new DzEntryPointNg(node.top()
				, returnType
				, name
				, arguments
				);
		}

		return new DzFunctionNg(node.bottom()
			, name
			, arguments
			);
	}

	return defaultResult();
}

antlrcpp::Any VisitorV3::visitArgument(dzParser::ArgumentContext *context)
{
	auto name = context->ID()->getText();
	auto type = context->typeName()->accept(this)
		.as<DzTypeNameNg *>();

	return new DzMemberNg(name, type);
}

antlrcpp::Any VisitorV3::visitTypeName(dzParser::TypeNameContext *context)
{
	return new DzTypeNameNg(context->ID()->getText());
}

antlrcpp::Any VisitorV3::visitBlock(dzParser::BlockContext *context)
{
	return visit(context->ret());
}

antlrcpp::Any VisitorV3::visitRet(dzParser::RetContext *context)
{
	auto n = visit(context->value).as<DzNodeNg>();

	auto ret = new DzReturnNg(n.top(), nullptr);

	return DzNodeNg(ret, ret);
}

antlrcpp::Any VisitorV3::visitConstant(dzParser::ConstantContext *context)
{
	auto constant = new DzConstantNg(context->INT()->getText());

	return DzNodeNg(constant, constant);
}

antlrcpp::Any VisitorV3::visitCall(dzParser::CallContext *context)
{
	std::vector<DzValueNg *> parameters;

	for (auto expression : context->expression())
	{
		auto parameter = visit(expression)
			.as<DzNodeNg>();

		parameters.push_back(parameter.top());
	}

	auto call = new DzCallNg(context->ID()->getText(), parameters);

	return DzNodeNg(call, call);
}

antlrcpp::Any VisitorV3::visitMember(dzParser::MemberContext *context)
{
	auto member = new DzMemberAccessNg(context->ID()->getText());

	return DzNodeNg(member, member);
}

antlrcpp::Any VisitorV3::visitBinary(dzParser::BinaryContext *context)
{
	auto left = visit(context->left)
		.as<DzNodeNg>();

	auto right = visit(context->right)
		.as<DzNodeNg>();

	auto binary = new DzBinaryNg(context->op->getText()
		, left.top()
		, right.top()
		);

	return DzNodeNg(binary, binary);
}
