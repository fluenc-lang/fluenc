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

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include "antlr4-runtime/dzBaseVisitor.h"
#include "antlr4-runtime/dzLexer.h"
#include "antlr4-runtime/dzParser.h"

using namespace llvm;
using namespace std;
using namespace antlr4;

class CompilerException : public std::exception
{
	public:
		CompilerException(ParserRuleContext *context)
			: m_context(context)
		{
		}

		int row() const
		{
			return m_context->start->getLine();
		}

		int column() const
		{
			return m_context->start->getCharPositionInLine();
		}

		int length() const
		{
			return m_context->start->getStopIndex() - m_context->start->getStartIndex();
		}

		virtual std::string message() const = 0;

	private:
		ParserRuleContext *m_context;
};

class UnknownTypeException : public CompilerException
{
	public:
		UnknownTypeException(ParserRuleContext *context, const std::string &typeName)
			: CompilerException(context)
			, m_typeName(typeName)
		{
		}

		std::string message() const override
		{
			std::stringstream stream;
			stream << "Unknown type name '" << m_typeName << "'";

			return  stream.str();
		}

	private:
		std::string m_typeName;
};

class UndeclaredIdentifierException : public CompilerException
{
	public:
		UndeclaredIdentifierException(ParserRuleContext *context, const std::string &identifier)
			: CompilerException(context)
			, m_identifier(identifier)
		{
		}

		std::string message() const override
		{
			std::stringstream stream;
			stream << "'" << m_identifier << "' was not declared in this scope";

			return  stream.str();
		}

	private:
		std::string m_identifier;
};

class Visitor : public dzBaseVisitor
{
	public:
		Visitor(LLVMContext &context, Module &module, BasicBlock *block, Function *function, std::map<std::string, Value *> locals)
			: m_context(context)
			, m_module(module)
			, m_block(block)
			, m_function(function)
			, m_locals(locals)
		{
		}

		antlrcpp::Any visitAssignment(dzParser::AssignmentContext *context) override
		{
			IRBuilder<> builder(m_block);

			auto name = context->ID()->getText();
			auto expression = context->expression();

			auto value = visitAs<Value *>(expression);

			auto alloc = builder.CreateAlloca(value->getType());

			builder.CreateStore(value, alloc);

			return builder.CreateLoad(alloc, name);
		}

		antlrcpp::Any visitFunction(dzParser::FunctionContext *context) override
		{
			auto returnType = visitAs<Type *>(context->typeName());
			auto arguments = context->argument();
			auto name = context->ID()->getText();

			std::vector<Type *> argumentTypes;
			std::map<std::string, Value *> locals;

			for (auto argument : arguments)
			{
				auto argumentType = visitAs<Type *>(argument->typeName());

				argumentTypes.push_back(argumentType);
			}

			auto functionType = FunctionType::get(returnType, argumentTypes, false);
			auto function = Function::Create(functionType, Function::ExternalLinkage, name, m_module);

			for (auto i = 0u; i < arguments.size(); i++)
			{
				auto argument = arguments[i];
				auto name = argument->ID()->getText();

				locals[name] = function->getArg(i);
			}

			Visitor child(m_context, m_module, m_block, function, locals);

			child.visit(context->block());

			verifyFunction(*function);

			return function;
		}

		antlrcpp::Any visitTypeName(dzParser::TypeNameContext *context) override
		{
			auto typeName = context->ID()->getText();

			if (typeName == "int")
			{
				return (Type *)Type::getInt32Ty(m_context);
			}

			throw new UnknownTypeException(context, typeName);
		}

		Visitor populateBlock(Visitor visitor
			, std::vector<dzParser::StatementContext *>::const_iterator iterator
			, std::vector<dzParser::StatementContext *>::const_iterator end
			)
		{
			if (iterator == end)
			{
				return visitor;
			}

			auto result = visitor.visit(*iterator);

			if (result.is<BasicBlock *>())
			{
				auto block = result.as<BasicBlock *>();

				Visitor child(m_context, m_module, block, m_function, visitor.m_locals);

				return populateBlock(child, next(iterator), end);
			}

			if (result.is<LoadInst *>())
			{
				auto local = result.as<LoadInst *>();

				auto locals = visitor.m_locals;

				locals[local->getName().str()] = local;

				Visitor child(m_context, m_module, visitor.m_block, m_function, locals);

				return populateBlock(child, next(iterator), end);
			}

			return populateBlock(visitor, next(iterator), end);
		}

		antlrcpp::Any visitBlock(dzParser::BlockContext *context) override
		{
			auto outer = BasicBlock::Create(m_context, "entry", m_function);

			Visitor visitor(m_context, m_module, outer, m_function, m_locals);

			auto statements = context->statement();

			auto inner = populateBlock(visitor
				, begin(statements)
				, end(statements)
				);

			inner.visit(context->ret());

			return outer;
		}

		antlrcpp::Any visitRet(dzParser::RetContext *context) override
		{
			IRBuilder<> builder(m_block);

			auto value = visitAs<Value *>(context->expression());

			return builder.CreateRet(value);
		}

		antlrcpp::Any visitConstant(dzParser::ConstantContext *context) override
		{
			auto value = context->INT()->toString();
			auto constant = (Value *)ConstantInt::get(Type::getInt32Ty(m_context), value, 10);

			return constant;
		}

		antlrcpp::Any visitMember(dzParser::MemberContext *context) override
		{
			auto name = context->getText();

			auto result = m_locals.find(name);

			if (result == m_locals.end())
			{
				throw new UndeclaredIdentifierException(context, name);
			}

			return result->second;
		}

		antlrcpp::Any visitCall(dzParser::CallContext *context) override
		{
			IRBuilder<> builder(m_block);

			auto name = context->ID()->getText();
			auto target = m_module.getFunction(name);

			if (!target)
			{
				throw new UndeclaredIdentifierException(context, name);
			}

			std::vector<Value *> arguments;

			for (auto argument : context->expression())
			{
				auto value = visitAs<Value *>(argument);

				arguments.push_back(value);
			}

			return (Value *)builder.CreateCall(target, arguments);
		}

		antlrcpp::Any visitBinary(dzParser::BinaryContext *context) override
		{
			IRBuilder<> builder(m_block);

			auto left = visitAs<Value *>(context->left);
			auto right = visitAs<Value *>(context->right);

			if (context->op->getText() == "+")
			{
				return builder.CreateAdd(left, right);
			}

			if (context->op->getText() == "-")
			{
				return builder.CreateSub(left, right);
			}

			if (context->op->getText() == "*")
			{
				return builder.CreateMul(left, right);
			}

			if (context->op->getText() == "/")
			{
				return builder.CreateSDiv(left, right);
			}

			if (context->op->getText() == "<")
			{
				return builder.CreateCmp(CmpInst::Predicate::ICMP_SLT, left, right);
			}

			if (context->op->getText() == "<=")
			{
				return builder.CreateCmp(CmpInst::Predicate::ICMP_SLE, left, right);
			}

			if (context->op->getText() == ">")
			{
				return builder.CreateCmp(CmpInst::Predicate::ICMP_SGT, left, right);
			}

			if (context->op->getText() == ">=")
			{
				return builder.CreateCmp(CmpInst::Predicate::ICMP_SGE, left, right);
			}

			if (context->op->getText() == "==")
			{
				return builder.CreateCmp(CmpInst::Predicate::ICMP_EQ, left, right);
			}

			if (context->op->getText() == "!=")
			{
				return builder.CreateCmp(CmpInst::Predicate::ICMP_NE, left, right);
			}

			return nullptr;
		}

		antlrcpp::Any visitConditional(dzParser::ConditionalContext *context) override
		{
			IRBuilder<> builder(m_block);

			auto condition = visitAs<Value *>(context->expression());

			auto ifTrue = visitAs<BasicBlock *>(context->block());
			auto ifFalse = BasicBlock::Create(m_context, "ifFalse", m_function);

			builder.CreateCondBr(condition, ifTrue, ifFalse);

			return ifFalse;
		}

	private:
		template<class T>
		T visitAs(tree::ParseTree *tree)
		{
			return tree->accept(this).as<T>();
		}

		LLVMContext &m_context;
		Module &m_module;
		BasicBlock *m_block;
		Function *m_function;
		std::map<std::string, Value *> m_locals;
};


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

	Visitor visitor(context, module, nullptr, nullptr, std::map<std::string, Value *>());

	try
	{
		program->accept(&visitor);
	}
	catch (CompilerException *exception)
	{
		std::cout << "main.dz:" << exception->row() << ":" << exception->column() << ": error: " << exception->message() << std::endl;
		std::cout << std::setw(5) << exception->row() << " | ";

		stream.seekg(0);

		std::string line;

		for (int i = 0; i < exception->row(); i++)
		{
			std::getline(stream, line);
		}

		auto trimmed = std::find_if(begin(line), end(line), [](unsigned char ch)
		{
			return !std::isspace(ch);
		});

		for (auto i = trimmed; i != end(line); i++)
		{
			std::cout << *i;
		}

		std::cout << std::endl;
		std::cout << std::setw(8) << "| ";

		for (auto i = trimmed; i != end(line); i++)
		{
			auto tokenSelector = [&]
			{
				if (i == begin(line) + exception->column())
				{
					return "^";
				}

				if (i < begin(line) + exception->column())
				{
					return " ";
				}

				if (i > begin(line) + exception->column() + exception->length())
				{
					return " ";
				}

				return "~";
			};

			std::cout << tokenSelector();
		}

		std::cout << std::endl;

		return 1;
	}

	module.print(llvm::errs(), nullptr);

	std::string error;

	auto targetTriple = sys::getDefaultTargetTriple();
	auto target = TargetRegistry::lookupTarget(targetTriple, error);

	if (!target)
	{
		errs() << error;

		return 1;
	}

	auto relocModel = Optional<Reloc::Model>();
	auto targetMachine = target->createTargetMachine(targetTriple, "generic", "", TargetOptions(), relocModel);

	module.setDataLayout(targetMachine->createDataLayout());

	std::error_code EC;
	raw_fd_ostream dest("output.o", EC, sys::fs::OF_None);

	legacy::PassManager pm;

	targetMachine->addPassesToEmitFile(pm, dest, nullptr, CGFT_ObjectFile);

	pm.run(module);

	dest.flush();

	return 0;

}
