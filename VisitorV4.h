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

class Type;

class VisitorV4 : public dzBaseVisitor
{
	public:
		VisitorV4(const Type *iteratorType, DzValue *alpha, DzValue *beta);

		antlrcpp::Any visitProgram(dzParser::ProgramContext *context) override;
		antlrcpp::Any visitFunction(dzParser::FunctionContext *context) override;
		antlrcpp::Any visitRegularType(dzParser::RegularTypeContext *context) override;
		antlrcpp::Any visitFunctionType(dzParser::FunctionTypeContext *context) override;
		antlrcpp::Any visitStandardArgument(dzParser::StandardArgumentContext *context) override;
		antlrcpp::Any visitTupleArgument(dzParser::TupleArgumentContext *context) override;
		antlrcpp::Any visitRet(dzParser::RetContext *context) override;
		antlrcpp::Any visitBlock(dzParser::BlockContext *context) override;
		antlrcpp::Any visitBinary(dzParser::BinaryContext *context) override;
		antlrcpp::Any visitCall(dzParser::CallContext *context) override;
		antlrcpp::Any visitWith(dzParser::WithContext *context) override;
		antlrcpp::Any visitMember(dzParser::MemberContext *context) override;
		antlrcpp::Any visitInt32Literal(dzParser::Int32LiteralContext *context) override;
		antlrcpp::Any visitInt64Literal(dzParser::Int64LiteralContext *context) override;
		antlrcpp::Any visitBoolLiteral(dzParser::BoolLiteralContext *context) override;
		antlrcpp::Any visitStringLiteral(dzParser::StringLiteralContext *context) override;
		antlrcpp::Any visitUint32Literal(dzParser::Uint32LiteralContext *context) override;
		antlrcpp::Any visitStructure(dzParser::StructureContext *context) override;
		antlrcpp::Any visitInstantiation(dzParser::InstantiationContext *context) override;
		antlrcpp::Any visitConditional(dzParser::ConditionalContext *context) override;
		antlrcpp::Any visitGlobal(dzParser::GlobalContext *context) override;
		antlrcpp::Any visitNothing(dzParser::NothingContext *context) override;
		antlrcpp::Any visitGroup(dzParser::GroupContext *context) override;
		antlrcpp::Any visitExpansion(dzParser::ExpansionContext *context) override;
		antlrcpp::Any visitContinuation(dzParser::ContinuationContext *context) override;
		antlrcpp::Any visitArray(dzParser::ArrayContext *context) override;
		antlrcpp::Any visitCharLiteral(dzParser::CharLiteralContext *context) override;
		antlrcpp::Any visitByteLiteral(dzParser::ByteLiteralContext *context) override;
		antlrcpp::Any visitLocal(dzParser::LocalContext *context) override;

		template<typename T>
		T visit(antlr4::tree::ParseTree *tree)
		{
			auto result = dzBaseVisitor::visit(tree);

			return result.as<T>();
		}

		template<typename T, typename TResult>
		TResult visit(antlr4::tree::ParseTree *tree)
		{
			return dynamic_cast<TResult>(visit<T>(tree));
		}

	private:
		const Type *m_iteratorType;

		DzValue *m_alpha;
		DzValue *m_beta;
};

#endif // VISITORV4_H
