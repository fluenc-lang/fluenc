#ifndef VISITORV2_H
#define VISITORV2_H

#include "antlr4-runtime/dzBaseVisitor.h"

#include "FunctionAttribute.h"

class DzValue;

class Node
{
	public:
		Node(DzValue *value, DzValue *root)
			: m_value(value)
			, m_root(root)
		{
		}

		DzValue *value() const
		{
			return m_value;
		}

		DzValue *root() const
		{
			return m_root;
		}

	private:
		DzValue *m_value;
		DzValue *m_root;
};

class VisitorV2 : public dzBaseVisitor
{
	public:
		VisitorV2(FunctionAttribute attribute
			, DzValue *parent
			, DzValue *deepest
			);

		antlrcpp::Any visitBlock(dzParser::BlockContext *ctx) override;
		antlrcpp::Any visitBinary(dzParser::BinaryContext *ctx) override;
		antlrcpp::Any visitMember(dzParser::MemberContext *ctx) override;
		antlrcpp::Any visitConstant(dzParser::ConstantContext *ctx) override;
		antlrcpp::Any visitCall(dzParser::CallContext *ctx) override;
		antlrcpp::Any visitFunction(dzParser::FunctionContext *ctx) override;
		antlrcpp::Any visitProgram(dzParser::ProgramContext *ctx) override;
		antlrcpp::Any visitRet(dzParser::RetContext *ctx) override;
		antlrcpp::Any visitTypeName(dzParser::TypeNameContext *ctx) override;
		antlrcpp::Any visitArgument(dzParser::ArgumentContext *ctx) override;

	private:
		FunctionAttribute m_attribute;

		DzValue *m_parent;
		DzValue *m_deepest;
};

#endif // VISITORV2_H
