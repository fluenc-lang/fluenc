
// Generated from dz.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"
#include "dzParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by dzParser.
 */
class  dzVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by dzParser.
   */
    virtual antlrcpp::Any visitProgram(dzParser::ProgramContext *context) = 0;

    virtual antlrcpp::Any visitStructure(dzParser::StructureContext *context) = 0;

    virtual antlrcpp::Any visitGlobal(dzParser::GlobalContext *context) = 0;

    virtual antlrcpp::Any visitAssignment(dzParser::AssignmentContext *context) = 0;

    virtual antlrcpp::Any visitField(dzParser::FieldContext *context) = 0;

    virtual antlrcpp::Any visitFunction(dzParser::FunctionContext *context) = 0;

    virtual antlrcpp::Any visitInt32Literal(dzParser::Int32LiteralContext *context) = 0;

    virtual antlrcpp::Any visitInt64Literal(dzParser::Int64LiteralContext *context) = 0;

    virtual antlrcpp::Any visitBoolLiteral(dzParser::BoolLiteralContext *context) = 0;

    virtual antlrcpp::Any visitStringLiteral(dzParser::StringLiteralContext *context) = 0;

    virtual antlrcpp::Any visitUint32Literal(dzParser::Uint32LiteralContext *context) = 0;

    virtual antlrcpp::Any visitNothing(dzParser::NothingContext *context) = 0;

    virtual antlrcpp::Any visitWith(dzParser::WithContext *context) = 0;

    virtual antlrcpp::Any visitCall(dzParser::CallContext *context) = 0;

    virtual antlrcpp::Any visitConstant(dzParser::ConstantContext *context) = 0;

    virtual antlrcpp::Any visitConditional(dzParser::ConditionalContext *context) = 0;

    virtual antlrcpp::Any visitArray(dzParser::ArrayContext *context) = 0;

    virtual antlrcpp::Any visitBinary(dzParser::BinaryContext *context) = 0;

    virtual antlrcpp::Any visitMember(dzParser::MemberContext *context) = 0;

    virtual antlrcpp::Any visitInstantiation(dzParser::InstantiationContext *context) = 0;

    virtual antlrcpp::Any visitGroup(dzParser::GroupContext *context) = 0;

    virtual antlrcpp::Any visitRet(dzParser::RetContext *context) = 0;

    virtual antlrcpp::Any visitBlock(dzParser::BlockContext *context) = 0;

    virtual antlrcpp::Any visitArgument(dzParser::ArgumentContext *context) = 0;

    virtual antlrcpp::Any visitTypeName(dzParser::TypeNameContext *context) = 0;


};

