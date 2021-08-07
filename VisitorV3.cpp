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
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/RTDyldMemoryManager.h"
#include "llvm/ExecutionEngine/RuntimeDyld.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/ObjectLinkingLayer.h"
#include "llvm/ExecutionEngine/JITSymbol.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Mangler.h"
#include "llvm/Support/DynamicLibrary.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetSelect.h"

class DzTypeNameNg;
class DzCallable;
class DzValueNg;

class EntryPointInfo
{
	public:
		EntryPointInfo(DzTypeNameNg *returnType
			, std::unique_ptr<llvm::LLVMContext> &context
			, std::unique_ptr<llvm::Module> &module
			, llvm::BasicBlock *block
			, llvm::Function *function
			, const std::map<std::string, DzCallable *> &functions
			, const std::map<std::string, llvm::Value *> &locals
			, const std::string &name
			)
			: m_returnType(returnType)
			, m_context(context)
			, m_module(module)
			, m_block(block)
			, m_function(function)
			, m_functions(functions)
			, m_locals(locals)
			, m_name(name)
		{
		}

		DzTypeNameNg *returnType() const
		{
			return m_returnType;
		}

		std::unique_ptr<llvm::LLVMContext> &context() const
		{
			return m_context;
		}

		std::unique_ptr<llvm::Module> &module() const
		{
			return m_module;
		}

		llvm::BasicBlock *block() const
		{
			return m_block;
		}

		llvm::Function *function() const
		{
			return m_function;
		}

		std::map<std::string, DzCallable *> functions() const
		{
			return m_functions;
		}

		std::map<std::string, llvm::Value *> locals() const
		{
			return m_locals;
		}

		std::string name() const
		{
			return m_name;
		}

		EntryPointInfo withBlock(llvm::BasicBlock *block) const
		{
			return EntryPointInfo(m_returnType
				, m_context
				, m_module
				, block
				, m_function
				, m_functions
				, m_locals
				, m_name
				);
		}

		EntryPointInfo withFunction(llvm::Function *function) const
		{
			return EntryPointInfo(m_returnType
				, m_context
				, m_module
				, m_block
				, function
				, m_functions
				, m_locals
				, m_name
				);
		}

		EntryPointInfo withLocals(const std::map<std::string, llvm::Value *> &locals) const
		{
			return EntryPointInfo(m_returnType
				, m_context
				, m_module
				, m_block
				, m_function
				, m_functions
				, locals
				, m_name
				);
		}

		EntryPointInfo withName(const std::string &name) const
		{
			return EntryPointInfo(m_returnType
				, m_context
				, m_module
				, m_block
				, m_function
				, m_functions
				, m_locals
				, name
				);
		}

	private:
		DzTypeNameNg *m_returnType;

		std::unique_ptr<llvm::LLVMContext> &m_context;
		std::unique_ptr<llvm::Module> &m_module;

		llvm::BasicBlock *m_block;
		llvm::Function *m_function;

		std::map<std::string, DzCallable *> m_functions;
		std::map<std::string, llvm::Value *> m_locals;

		std::string m_name;
};

class DzValueNg
{
	public:
		virtual llvm::Value *build(const EntryPointInfo &entryPoint) const = 0;
};

class DzMemberNg
{
	public:
		DzMemberNg(const std::string &name, DzTypeNameNg *type)
			: m_name(name)
			, m_type(type)
		{
		}

		std::string name() const
		{
			return m_name;
		}

		DzTypeNameNg *type() const
		{
			return m_type;
		}

	private:
		std::string m_name;

		DzTypeNameNg *m_type;
};

class DzTypeNameNg
{
	public:
		DzTypeNameNg(const std::string &name)
			: m_name(name)
		{
		}

		llvm::Type *build(const EntryPointInfo &entryPoint) const
		{
			auto &context = entryPoint.context();

			if (m_name == "int")
			{
				return llvm::Type::getInt32Ty(*context);
			}

			return nullptr;
		}

	private:
		std::string m_name;
};

class DzCallable : public DzValueNg
{
	public:
		virtual std::string name() const = 0;
		virtual std::vector<DzMemberNg *> arguments() const = 0;

		virtual DzTypeNameNg *returnType() const = 0;

		virtual FunctionAttribute attribute() const = 0;
};

class DzEntryPointNg : public DzCallable
{
	public:
		DzEntryPointNg(DzValueNg *block
			, DzTypeNameNg *returnType
			, const std::string &name
			, const std::vector<DzMemberNg *> &arguments
			)
			: m_block(block)
			, m_returnType(returnType)
			, m_name(name)
			, m_arguments(arguments)
		{
		}

		std::string name() const override
		{
			return m_name;
		}

		std::vector<DzMemberNg *> arguments() const override
		{
			return m_arguments;
		}

		FunctionAttribute attribute() const override
		{
			return FunctionAttribute::Export;
		}

		DzTypeNameNg *returnType() const override
		{
			return m_returnType;
		}

		llvm::Value *build(const EntryPointInfo &entryPoint) const override
		{
			auto returnType = entryPoint.returnType()->build(entryPoint);
			auto &module = entryPoint.module();
			auto &context = entryPoint.context();

			std::vector<llvm::Type *> argumentTypes;

			for (auto i = 0u; i < m_arguments.size(); i++)
			{
				auto argument = m_arguments[i];

//				auto name = argument->name();
				auto type = argument->type()->build(entryPoint);

		//		locals[name] = new DzLocalAccess(i);

				argumentTypes.push_back(type);
			}

			auto functionType = llvm::FunctionType::get(returnType, argumentTypes, false);
			auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, m_name, module.get());
			auto block = llvm::BasicBlock::Create(*context, "entry", function);

			auto ep = entryPoint
				.withBlock(block)
				.withFunction(function)
				.withName(m_name);

			auto value = m_block->build(ep);

			llvm::IRBuilder<> builder(block);

			auto load = builder.CreateLoad(llvm::Type::getInt32Ty(*context), value);

			builder.CreateRet(load);

			for (auto i = function->begin(), j = std::next(i)
				; i != function->end() && j != function->end()
				; i++, j++
				)
			{
				llvm::BranchInst::Create(&*j, &*i);
			}

			verifyFunction(*function);

			return function;
		}

	private:
		DzValueNg *m_block;
		DzTypeNameNg *m_returnType;

		std::string m_name;
		std::vector<DzMemberNg *> m_arguments;
};

class DzFunctionNg : public DzCallable
{
	public:
		DzFunctionNg(DzValueNg *block
			, const std::string &name
			, const std::vector<DzMemberNg *> arguments
			)
			: m_name(name)
			, m_arguments(arguments)
			, m_block(block)
		{
		}

		std::string name() const override
		{
			return m_name;
		}

		std::vector<DzMemberNg *> arguments() const override
		{
			return m_arguments;
		}

		FunctionAttribute attribute() const override
		{
			return FunctionAttribute::None;
		}

		DzTypeNameNg *returnType() const override
		{
			return nullptr;
		}

		llvm::Value *build(const EntryPointInfo &entryPoint) const override
		{
			auto &context = entryPoint.context();
			auto function = entryPoint.function();
			auto parent = entryPoint.block();

			auto block = llvm::BasicBlock::Create(*context, m_name, function, parent);

			llvm::IRBuilder<> builder(block);

			auto ep = entryPoint
				.withBlock(block)
				.withName(block->getName().str());

			return m_block->build(ep);
		}

	private:
		std::string m_name;
		std::vector<DzMemberNg *> m_arguments;

		DzValueNg *m_block;
};

class DzNodeNg
{
	public:
		DzNodeNg(DzValueNg *top, DzValueNg *bottom)
			: m_top(top)
			, m_bottom(bottom)
		{
		}

		DzValueNg *top() const
		{
			return m_top;
		}

		DzValueNg *bottom() const
		{
			return m_bottom;
		}

	private:
		DzValueNg *m_top;
		DzValueNg *m_bottom;
};

VisitorV3::VisitorV3()
{

}

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

class DzReturnNg : public DzValueNg
{
	public:
		DzReturnNg(DzValueNg *value)
			: m_value(value)
		{
		}

		llvm::Value *build(const EntryPointInfo &entryPoint) const override
		{
			auto &module = entryPoint.module();
			auto &context = entryPoint.context();

			std::ostringstream ss;
			ss << entryPoint.name();
			ss << "_ret";

			auto k = ss.str();

			auto c = module->getOrInsertGlobal(k, llvm::Type::getInt32Ty(*context), [&]
			{
				auto initializer = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 0);

				return new llvm::GlobalVariable(*module, llvm::Type::getInt32Ty(*context), false, llvm::GlobalValue::InternalLinkage, initializer, k);
			});

			auto v = m_value->build(entryPoint);

			llvm::IRBuilder<> builder(entryPoint.block());
			builder.CreateStore(v, c);

			return c;
		}

	private:
		DzValueNg *m_value;
};

antlrcpp::Any VisitorV3::visitRet(dzParser::RetContext *context)
{
	auto n = visit(context->value).as<DzNodeNg>();

	auto ret = new DzReturnNg(n.top());

	return DzNodeNg(ret, ret);
}

class DzConstantNg : public DzValueNg
{
	public:
		DzConstantNg(const std::string &value)
			: m_value(value)
		{
		}

		llvm::Value *build(const EntryPointInfo &entryPoint) const override
		{
			auto type = llvm::Type::getInt32Ty(*entryPoint.context());

			return llvm::ConstantInt::get(type, m_value, 10);
		}

	private:
		std::string m_value;
};

antlrcpp::Any VisitorV3::visitConstant(dzParser::ConstantContext *context)
{
	auto constant = new DzConstantNg(context->INT()->getText());

	return DzNodeNg(constant, constant);
}

class DzCallNg : public DzValueNg
{
	public:
		DzCallNg(const std::string &name, std::vector<DzValueNg *> parameters)
			: m_name(name)
			, m_parameters(parameters)
		{
		}

		llvm::Value *build(const EntryPointInfo &entryPoint) const override
		{
			auto &context = entryPoint.context();
			auto functions = entryPoint.functions();
			auto block = entryPoint.block();

			auto iterator = functions.find(m_name);

			if (iterator == functions.end())
			{
				return nullptr;
			}

			auto function = iterator->second;

			auto locals = entryPoint.locals();

			auto arg = function->arguments();

			for (auto i = 0u; i < m_parameters.size() && i < arg.size(); i++)
			{
				auto name = arg[i]->name();

				locals[name] = m_parameters[i]->build(entryPoint);
			}

			auto ep = entryPoint
				.withLocals(locals);

			auto addressOfReturnValue = iterator->second->build(ep);

			llvm::IRBuilder<> builder(block);

			return builder.CreateLoad(llvm::Type::getInt32Ty(*context), addressOfReturnValue);
		}

	private:
		std::string m_name;
		std::vector<DzValueNg *> m_parameters;
};

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

class DzMemberAccessNg : public DzValueNg
{
	public:
		DzMemberAccessNg(const std::string &name)
			: m_name(name)
		{
		}

		llvm::Value *build(const EntryPointInfo &entryPoint) const override
		{
			const auto &locals = entryPoint.locals();

			auto iterator = locals.find(m_name);

			if (iterator == locals.end())
			{
				return nullptr;
			}

			return iterator->second;
		}

	private:
		std::string m_name;
};

antlrcpp::Any VisitorV3::visitMember(dzParser::MemberContext *context)
{
	auto member = new DzMemberAccessNg(context->ID()->getText());

	return DzNodeNg(member, member);
}

class DzBinaryNg : public DzValueNg
{
	public:
		DzBinaryNg(const std::string &op
			, DzValueNg *left
			, DzValueNg *right
			)
			: m_op(op)
			, m_left(left)
			, m_right(right)
		{
		}

		llvm::Value *build(const EntryPointInfo &entryPoint) const override
		{
			auto left = m_left->build(entryPoint);
			auto right = m_right->build(entryPoint);

			llvm::IRBuilder<> builder(entryPoint.block());

			return builder.CreateBinOp(llvm::Instruction::Add, left, right);
		}

	private:
		std::string m_op;

		DzValueNg *m_left;
		DzValueNg *m_right;
};

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
