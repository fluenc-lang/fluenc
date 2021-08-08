#ifndef VISITORV4_H
#define VISITORV4_H

#include "VisitorV3.h"

class DzValueMkII;

class VisitorV4 : public dzBaseVisitor
{
	public:
		VisitorV4(class DzValueMkII *consumer);

		antlrcpp::Any visitProgram(dzParser::ProgramContext *context) override;
		antlrcpp::Any visitFunction(dzParser::FunctionContext *context) override;
		antlrcpp::Any visitConstant(dzParser::ConstantContext *context) override;
		antlrcpp::Any visitRet(dzParser::RetContext *context) override;
		antlrcpp::Any visitBlock(dzParser::BlockContext *context) override;
		antlrcpp::Any visitBinary(dzParser::BinaryContext *context) override;
		antlrcpp::Any visitCall(dzParser::CallContext *context) override;
		antlrcpp::Any visitMember(dzParser::MemberContext *context) override;

	private:
		DzValueMkII *m_consumer;
};

#endif // VISITORV4_H
