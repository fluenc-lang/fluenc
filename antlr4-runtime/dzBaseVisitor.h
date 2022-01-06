
// Generated from dz.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"
#include "dzVisitor.h"


/**
 * This class provides an empty implementation of dzVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  dzBaseVisitor : public dzVisitor {
public:

  virtual antlrcpp::Any visitProgram(dzParser::ProgramContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStructure(dzParser::StructureContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGlobal(dzParser::GlobalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssignment(dzParser::AssignmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitField(dzParser::FieldContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunction(dzParser::FunctionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInt32Literal(dzParser::Int32LiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInt64Literal(dzParser::Int64LiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBoolLiteral(dzParser::BoolLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStringLiteral(dzParser::StringLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUint32Literal(dzParser::Uint32LiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCharLiteral(dzParser::CharLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitByteLiteral(dzParser::ByteLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNothing(dzParser::NothingContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitWith(dzParser::WithContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCall(dzParser::CallContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConstant(dzParser::ConstantContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConditional(dzParser::ConditionalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitArray(dzParser::ArrayContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBinary(dzParser::BinaryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMember(dzParser::MemberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInstantiation(dzParser::InstantiationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGroup(dzParser::GroupContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExpansion(dzParser::ExpansionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitContinuation(dzParser::ContinuationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRet(dzParser::RetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBlock(dzParser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStandardArgument(dzParser::StandardArgumentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTupleArgument(dzParser::TupleArgumentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTypeName(dzParser::TypeNameContext *ctx) override {
    return visitChildren(ctx);
  }


};

