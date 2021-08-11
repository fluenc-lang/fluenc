#ifndef VISITORV4_H
#define VISITORV4_H

#include <llvm/IR/Module.h>

#include "antlr4-runtime/dzBaseVisitor.h"

class DzValue;

class ModuleInfo
{
	public:
		ModuleInfo(std::unique_ptr<llvm::Module> module
			, std::unique_ptr<llvm::LLVMContext> context
			)
			: m_module(std::move(module))
			, m_context(std::move(context))
		{
		}

		std::unique_ptr<llvm::Module> &module()
		{
			return m_module;
		}

		std::unique_ptr<llvm::LLVMContext> &context()
		{
			return m_context;
		}

	private:
		std::unique_ptr<llvm::Module> m_module;
		std::unique_ptr<llvm::LLVMContext> m_context;
};

class VisitorV4 : public dzBaseVisitor
{
	public:
		VisitorV4(DzValue *consumer);

		antlrcpp::Any visitProgram(dzParser::ProgramContext *context) override;
		antlrcpp::Any visitFunction(dzParser::FunctionContext *context) override;
		antlrcpp::Any visitTypeName(dzParser::TypeNameContext *context) override;
		antlrcpp::Any visitArgument(dzParser::ArgumentContext *context) override;
		antlrcpp::Any visitConstant(dzParser::ConstantContext *context) override;
		antlrcpp::Any visitRet(dzParser::RetContext *context) override;
		antlrcpp::Any visitBlock(dzParser::BlockContext *context) override;
		antlrcpp::Any visitBinary(dzParser::BinaryContext *context) override;
		antlrcpp::Any visitCall(dzParser::CallContext *context) override;
		antlrcpp::Any visitMember(dzParser::MemberContext *context) override;

	private:
		DzValue *m_consumer;
};

#endif // VISITORV4_H
