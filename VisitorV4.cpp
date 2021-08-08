#include <deque>
#include <numeric>

#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>

#include "VisitorV4.h"
#include "FunctionAttribute.h"

class DzCallableMkII;

class EntryPointInfoMkII
{
	public:
		EntryPointInfoMkII(llvm::BasicBlock *block
			, llvm::Function *function
			, std::unique_ptr<llvm::Module> &module
			, std::unique_ptr<llvm::LLVMContext> &context
			, const std::map<std::string, DzCallableMkII *> &functions
			, const std::map<std::string, llvm::Value *> &locals
			)
			: m_block(block)
			, m_function(function)
			, m_module(module)
			, m_context(context)
			, m_functions(functions)
			, m_locals(locals)
		{
		}

		llvm::BasicBlock *block() const
		{
			return m_block;
		}

		llvm::Function *function() const
		{
			return m_function;
		}

		std::unique_ptr<llvm::Module> &module() const
		{
			return m_module;
		}

		std::unique_ptr<llvm::LLVMContext> &context() const
		{
			return m_context;
		}

		std::map<std::string, DzCallableMkII *> functions() const
		{
			return m_functions;
		}

		std::map<std::string, llvm::Value *> locals() const
		{
			return m_locals;
		}

		EntryPointInfoMkII withBlock(llvm::BasicBlock *block) const
		{
			return EntryPointInfoMkII(block
				, m_function
				, m_module
				, m_context
				, m_functions
				, m_locals
				);
		}

		EntryPointInfoMkII withFunction(llvm::Function *function) const
		{
			return EntryPointInfoMkII(m_block
				, function
				, m_module
				, m_context
				, m_functions
				, m_locals
				);
		}

		EntryPointInfoMkII withLocals(const std::map<std::string, llvm::Value *> &locals) const
		{
			return EntryPointInfoMkII(m_block
				, m_function
				, m_module
				, m_context
				, m_functions
				, locals
				);
		}

	private:
		llvm::BasicBlock *m_block;
		llvm::Function *m_function;

		std::unique_ptr<llvm::Module> &m_module;
		std::unique_ptr<llvm::LLVMContext> &m_context;

		std::map<std::string, DzCallableMkII *> m_functions;
		std::map<std::string, llvm::Value *> m_locals;
};

class DzValueMkII
{
	public:
		virtual llvm::Value *build(const EntryPointInfoMkII &entryPoint, std::deque<llvm::Value *> &values) const = 0;
};


class DzCallableMkII : public DzValueMkII
{
	public:
		virtual std::string name() const = 0;

		virtual FunctionAttribute attribute() const = 0;
};

class DzEntryPointMkII : public DzCallableMkII
{
	public:
		DzEntryPointMkII(const std::string &name, DzValueMkII *block)
			: m_name(name)
			, m_block(block)
		{
		}

		std::string name() const override
		{
			return m_name;
		}

		FunctionAttribute attribute() const override
		{
			return FunctionAttribute::Export;
		}

		llvm::Value *build(const EntryPointInfoMkII &entryPoint, std::deque<llvm::Value *> &values) const override
		{
			auto &module = entryPoint.module();
			auto &context = entryPoint.context();

			auto returnType = llvm::Type::getInt32Ty(*context);

			std::vector<llvm::Type *> argumentTypes;

			auto functionType = llvm::FunctionType::get(returnType, argumentTypes, false);
			auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, m_name, module.get());
			auto block = llvm::BasicBlock::Create(*context, "entry");

			auto ep = entryPoint
				.withFunction(function)
				.withBlock(block);

			return m_block->build(ep, values);
		}

	private:
		std::string m_name;

		DzValueMkII *m_block;
};

class DzConstantMkII : public DzValueMkII
{
	public:
		DzConstantMkII(DzValueMkII *consumer, std::string value)
			: m_consumer(consumer)
			, m_value(value)
		{
		}

		llvm::Value *build(const EntryPointInfoMkII &entryPoint, std::deque<llvm::Value *> &values) const override
		{
			auto &context = entryPoint.context();

			auto value = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), m_value, 10);

			values.push_back(value);

			return m_consumer->build(entryPoint, values);
		}

	private:
		DzValueMkII *m_consumer;

		std::string m_value;
};

VisitorV4::VisitorV4(DzValueMkII *consumer)
	: m_consumer(consumer)
{
}

class DzEntryPointTerminatorMkII : public DzValueMkII
{
	public:
		llvm::Value *build(const EntryPointInfoMkII &entryPoint, std::deque<llvm::Value *> &values) const override
		{
			auto function = entryPoint.function();
			auto block = entryPoint.block();

			block->setName("entry");
			block->insertInto(function);

			llvm::IRBuilder<> builder(block);

			auto returnValue = values.back();

			values.pop_back();

			builder.CreateRet(returnValue);

			for (auto i = function->begin(), j = std::next(i)
				 ; i != function->end() && j != function->end()
				 ; i++, j++
				 )
			{
				llvm::BranchInst::Create(&*j, &*i);
			}

			return function;
		}
};

class DzFunctionTerminatorMkII : public DzValueMkII
{
	public:
		DzFunctionTerminatorMkII(const std::string &name)
			: m_name(name)
		{
		}

		llvm::Value *build(const EntryPointInfoMkII &entryPoint, std::deque<llvm::Value *> &values) const override
		{
			auto &context = entryPoint.context();
			auto &module = entryPoint.module();

			auto function = entryPoint.function();
			auto block = entryPoint.block();

			block->setName(m_name);
			block->insertInto(function);

			std::ostringstream stream;
			stream << m_name;
			stream << "_ret";

			auto globalName = stream.str();

			auto global = module->getOrInsertGlobal(globalName, llvm::Type::getInt32Ty(*context), [&]
			{
				auto initializer = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 0);

				return new llvm::GlobalVariable(*module, llvm::Type::getInt32Ty(*context), false, llvm::GlobalValue::InternalLinkage, initializer, globalName);
			});

			auto value = values.back();

			values.pop_back();

			llvm::IRBuilder<> builder(block);

			builder.CreateStore(value, global);

			auto load = builder.CreateLoad(llvm::Type::getInt32Ty(*context), global);

			return load;
		}

	private:
		std::string m_name;
};

class DzFunctionMkII : public DzCallableMkII
{
	public:
		DzFunctionMkII(const std::string &name
			, std::vector<std::string> arguments
			, DzValueMkII *block
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

		FunctionAttribute attribute() const override
		{
			return FunctionAttribute::None;
		}

		llvm::Value *build(const EntryPointInfoMkII &entryPoint, std::deque<llvm::Value *> &values) const override
		{
			std::map<std::string, llvm::Value *> locals;

			std::transform(begin(m_arguments), end(m_arguments), rbegin(values), std::inserter(locals, end(locals)), [](auto name, auto value)
			{
				return std::make_pair(name, value);
			});

			for (auto i = 0u; i < m_arguments.size(); i++)
			{
				values.pop_back();
			}

			auto ep = entryPoint
				.withLocals(locals);

			return m_block->build(ep, values);
		}

	private:
		std::string m_name;
		std::vector<std::string> m_arguments;

		DzValueMkII *m_block;
};

antlrcpp::Any VisitorV4::visitProgram(dzParser::ProgramContext *context)
{
	auto llvmContext = std::make_unique<llvm::LLVMContext>();
	auto module = std::make_unique<llvm::Module>("dz", *llvmContext);

	std::vector<DzCallableMkII *> roots;
	std::map<std::string, DzCallableMkII *> functions;
	std::map<std::string, llvm::Value *> locals;

	for (auto function : context->function())
	{
		auto result = visit(function)
			.as<DzCallableMkII *>();

		if (result->attribute() == FunctionAttribute::Export)
		{
			roots.push_back(result);
		}

		if (result->attribute() == FunctionAttribute::None)
		{
			functions[result->name()] = result;
		}
	}

	for (auto root : roots)
	{
		EntryPointInfoMkII entryPoint(nullptr, nullptr, module, llvmContext, functions, locals);

		std::deque<llvm::Value *> values;

		root->build(entryPoint, values);
	}

	module->print(llvm::errs(), nullptr);

	return new ModuleInfo(std::move(module), std::move(llvmContext));
}

FunctionAttribute getAttribute3(dzParser::FunctionContext *ctx)
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

class DzMemberMkII
{
	public:

};

antlrcpp::Any VisitorV4::visitFunction(dzParser::FunctionContext *context)
{
	auto attribute = getAttribute3(context);

	if (attribute == FunctionAttribute::Import)
	{
		return defaultResult();
	}

	auto name = context->name->getText();
	auto block = context->block();

	std::vector<std::string> arguments;

	for (auto argument : context->argument())
	{
		auto argumentName = argument->ID()->getText();

		arguments.push_back(argumentName);
	}

	if (attribute == FunctionAttribute::Export)
	{
		auto terminator = new DzEntryPointTerminatorMkII();

		VisitorV4 visitor(terminator);

		auto entryPoint = new DzEntryPointMkII(name
			, visitor.visit(block)
			);

		return static_cast<DzCallableMkII *>(entryPoint);
	}

	auto terminator = new DzFunctionTerminatorMkII(name);

	VisitorV4 visitor(terminator);

	auto function = new DzFunctionMkII(name
		, arguments
		, visitor.visit(block)
		);

	return static_cast<DzCallableMkII *>(function);
}

antlrcpp::Any VisitorV4::visitConstant(dzParser::ConstantContext *context)
{
	auto constant = new DzConstantMkII(m_consumer
		, context->INT()->getText()
		);

	return static_cast<DzValueMkII *>(constant);
}

antlrcpp::Any VisitorV4::visitRet(dzParser::RetContext *context)
{
	return visit(context->value);
}

antlrcpp::Any VisitorV4::visitBlock(dzParser::BlockContext *context)
{
	return visit(context->ret());
}

class DzBinaryMkII : public DzValueMkII
{
	public:
		DzBinaryMkII(DzValueMkII *consumer, const std::string &op)
			: m_consumer(consumer)
			, m_op(op)
		{
		}

		llvm::Instruction::BinaryOps resolveOp() const
		{
			if (m_op == "+")
			{
				return llvm::Instruction::Add;
			}

			if (m_op == "*")
			{
				return llvm::Instruction::Mul;
			}

			return llvm::Instruction::Add; // TODO
		}

		llvm::Value *build(const EntryPointInfoMkII &entryPoint, std::deque<llvm::Value *> &values) const override
		{
			auto block = entryPoint.block();

			auto left = values.back();

			values.pop_back();

			auto right = values.back();

			values.pop_back();

			llvm::IRBuilder<> builder(block);

			auto op = builder.CreateBinOp(resolveOp(), left, right);

			values.push_back(op);

			return m_consumer->build(entryPoint, values);
		}

	private:
		DzValueMkII *m_consumer;

		std::string m_op;
};

antlrcpp::Any VisitorV4::visitBinary(dzParser::BinaryContext *context)
{
	auto binary = new DzBinaryMkII(m_consumer
		, context->OP()->getText()
		);

	VisitorV4 leftVisitor(binary);

	auto left = leftVisitor
		.visit(context->left)
		.as<DzValueMkII *>();

	VisitorV4 rightVisitor(left);

	auto right = rightVisitor.visit(context->right);

	return right;
}

class DzCallMkII : public DzValueMkII
{
	public:
		DzCallMkII(DzValueMkII *consumer
			, const std::string name
			)
			: m_consumer(consumer)
			, m_name(name)
		{
		}

		llvm::Value *build(const EntryPointInfoMkII &entryPoint, std::deque<llvm::Value *> &values) const override
		{
			auto &context = entryPoint.context();
			auto functions = entryPoint.functions();

			auto iterator = functions.find(m_name);

			if (iterator == functions.end())
			{
				return nullptr;
			}

			auto function = iterator->second;

			auto returnValue = function->build(entryPoint, values);

			values.push_back(returnValue);

			auto block = llvm::BasicBlock::Create(*context);

			auto ep = entryPoint
				.withBlock(block);

			return m_consumer->build(ep, values);
		}

	private:
		DzValueMkII *m_consumer;

		std::string m_name;
};

antlrcpp::Any VisitorV4::visitCall(dzParser::CallContext *context)
{
	auto call = new DzCallMkII(m_consumer
		, context->ID()->getText()
		);

	auto expression = context->expression();

	auto value = std::accumulate(begin(expression), end(expression), (DzValueMkII *)call, [](DzValueMkII *consumer, dzParser::ExpressionContext *parameter)
	{
		VisitorV4 visitor(consumer);

		auto result = visitor
			.visit(parameter)
			.as<DzValueMkII *>();

		return result;
	});

	return static_cast<DzValueMkII *>(value);
}

class DzMemberAccessMkII : public DzValueMkII
{
	public:
		DzMemberAccessMkII(DzValueMkII *consumer, const std::string &name)
			: m_consumer(consumer)
			, m_name(name)
		{
		}

		llvm::Value *build(const EntryPointInfoMkII &entryPoint, std::deque<llvm::Value *> &values) const override
		{
			auto locals = entryPoint.locals();

			auto iterator = locals.find(m_name);

			if (iterator == locals.end())
			{
				return nullptr;
			}

			values.push_back(iterator->second);

			return m_consumer->build(entryPoint, values);
		}

	private:
		DzValueMkII *m_consumer;

		std::string m_name;
};

antlrcpp::Any VisitorV4::visitMember(dzParser::MemberContext *context)
{
	auto member = new DzMemberAccessMkII(m_consumer
		, context->ID()->getText()
		);

	return static_cast<DzValueMkII *>(member);
}

