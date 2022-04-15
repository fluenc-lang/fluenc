#ifndef VISITOR_H
#define VISITOR_H

#include "antlr4-runtime/fluencBaseVisitor.h"

class Node;
class Type;

class Visitor : public fluencBaseVisitor
{
	public:
		Visitor(const std::vector<std::string> &namespaces
			, const Type *iteratorType
			, const Node *alpha
			, const Node *beta
			);

		antlrcpp::Any visitProgram(fluencParser::ProgramContext *context) override;
		antlrcpp::Any visitFunction(fluencParser::FunctionContext *context) override;
		antlrcpp::Any visitRegularType(fluencParser::RegularTypeContext *context) override;
		antlrcpp::Any visitFunctionType(fluencParser::FunctionTypeContext *context) override;
		antlrcpp::Any visitStandardArgument(fluencParser::StandardArgumentContext *context) override;
		antlrcpp::Any visitTupleArgument(fluencParser::TupleArgumentContext *context) override;
		antlrcpp::Any visitRet(fluencParser::RetContext *context) override;
		antlrcpp::Any visitBlock(fluencParser::BlockContext *context) override;
		antlrcpp::Any visitBinary(fluencParser::BinaryContext *context) override;
		antlrcpp::Any visitCall(fluencParser::CallContext *context) override;
		antlrcpp::Any visitWith(fluencParser::WithContext *context) override;
		antlrcpp::Any visitMember(fluencParser::MemberContext *context) override;
		antlrcpp::Any visitInt32Literal(fluencParser::Int32LiteralContext *context) override;
		antlrcpp::Any visitInt64Literal(fluencParser::Int64LiteralContext *context) override;
		antlrcpp::Any visitBoolLiteral(fluencParser::BoolLiteralContext *context) override;
		antlrcpp::Any visitStringLiteral(fluencParser::StringLiteralContext *context) override;
		antlrcpp::Any visitUint32Literal(fluencParser::Uint32LiteralContext *context) override;
		antlrcpp::Any visitStructure(fluencParser::StructureContext *context) override;
		antlrcpp::Any visitInstantiation(fluencParser::InstantiationContext *context) override;
		antlrcpp::Any visitConditional(fluencParser::ConditionalContext *context) override;
		antlrcpp::Any visitGlobal(fluencParser::GlobalContext *context) override;
		antlrcpp::Any visitNothing(fluencParser::NothingContext *context) override;
		antlrcpp::Any visitGroup(fluencParser::GroupContext *context) override;
		antlrcpp::Any visitExpansion(fluencParser::ExpansionContext *context) override;
		antlrcpp::Any visitContinuation(fluencParser::ContinuationContext *context) override;
		antlrcpp::Any visitArray(fluencParser::ArrayContext *context) override;
		antlrcpp::Any visitCharLiteral(fluencParser::CharLiteralContext *context) override;
		antlrcpp::Any visitByteLiteral(fluencParser::ByteLiteralContext *context) override;
		antlrcpp::Any visitLocal(fluencParser::LocalContext *context) override;
		antlrcpp::Any visitInstruction(fluencParser::InstructionContext *context) override;
		antlrcpp::Any visitNs(fluencParser::NsContext *context) override;
		antlrcpp::Any visitUse(fluencParser::UseContext *context) override;

		antlrcpp::Any visitAny(antlr4::tree::ParseTree *tree)
		{
			return fluencBaseVisitor::visit(tree);
		}

		template<typename T>
		T visit(antlr4::tree::ParseTree *tree)
		{
			auto result = fluencBaseVisitor::visit(tree);

			return result.as<T>();
		}

		template<typename T, typename TResult>
		TResult visit(antlr4::tree::ParseTree *tree)
		{
			return dynamic_cast<TResult>(visit<T>(tree));
		}

	private:
		std::vector<std::string> m_namespaces;

		const Type *m_iteratorType;

		const Node *m_alpha;
		const Node *m_beta;
};

#endif // VISITOR_H
