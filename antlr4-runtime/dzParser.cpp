
// Generated from dz.g4 by ANTLR 4.9.2


#include "dzVisitor.h"

#include "dzParser.h"


using namespace antlrcpp;
using namespace antlr4;

dzParser::dzParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

dzParser::~dzParser() {
  delete _interpreter;
}

std::string dzParser::getGrammarFileName() const {
  return "dz.g4";
}

const std::vector<std::string>& dzParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& dzParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- ProgramContext ------------------------------------------------------------------

dzParser::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<dzParser::FunctionContext *> dzParser::ProgramContext::function() {
  return getRuleContexts<dzParser::FunctionContext>();
}

dzParser::FunctionContext* dzParser::ProgramContext::function(size_t i) {
  return getRuleContext<dzParser::FunctionContext>(i);
}

std::vector<dzParser::StructureContext *> dzParser::ProgramContext::structure() {
  return getRuleContexts<dzParser::StructureContext>();
}

dzParser::StructureContext* dzParser::ProgramContext::structure(size_t i) {
  return getRuleContext<dzParser::StructureContext>(i);
}


size_t dzParser::ProgramContext::getRuleIndex() const {
  return dzParser::RuleProgram;
}


antlrcpp::Any dzParser::ProgramContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitProgram(this);
  else
    return visitor->visitChildren(this);
}

dzParser::ProgramContext* dzParser::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 0, dzParser::RuleProgram);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(28);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == dzParser::T__0

    || _la == dzParser::ID) {
      setState(26);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case dzParser::ID: {
          setState(24);
          function();
          break;
        }

        case dzParser::T__0: {
          setState(25);
          structure();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(30);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StructureContext ------------------------------------------------------------------

dzParser::StructureContext::StructureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* dzParser::StructureContext::ID() {
  return getToken(dzParser::ID, 0);
}

std::vector<dzParser::FieldContext *> dzParser::StructureContext::field() {
  return getRuleContexts<dzParser::FieldContext>();
}

dzParser::FieldContext* dzParser::StructureContext::field(size_t i) {
  return getRuleContext<dzParser::FieldContext>(i);
}


size_t dzParser::StructureContext::getRuleIndex() const {
  return dzParser::RuleStructure;
}


antlrcpp::Any dzParser::StructureContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitStructure(this);
  else
    return visitor->visitChildren(this);
}

dzParser::StructureContext* dzParser::structure() {
  StructureContext *_localctx = _tracker.createInstance<StructureContext>(_ctx, getState());
  enterRule(_localctx, 2, dzParser::RuleStructure);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(31);
    match(dzParser::T__0);
    setState(32);
    match(dzParser::ID);
    setState(33);
    match(dzParser::T__1);
    setState(34);
    field();
    setState(39);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == dzParser::T__2) {
      setState(35);
      match(dzParser::T__2);
      setState(36);
      field();
      setState(41);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(42);
    match(dzParser::T__3);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignmentContext ------------------------------------------------------------------

dzParser::AssignmentContext::AssignmentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

dzParser::FieldContext* dzParser::AssignmentContext::field() {
  return getRuleContext<dzParser::FieldContext>(0);
}

dzParser::ExpressionContext* dzParser::AssignmentContext::expression() {
  return getRuleContext<dzParser::ExpressionContext>(0);
}


size_t dzParser::AssignmentContext::getRuleIndex() const {
  return dzParser::RuleAssignment;
}


antlrcpp::Any dzParser::AssignmentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitAssignment(this);
  else
    return visitor->visitChildren(this);
}

dzParser::AssignmentContext* dzParser::assignment() {
  AssignmentContext *_localctx = _tracker.createInstance<AssignmentContext>(_ctx, getState());
  enterRule(_localctx, 4, dzParser::RuleAssignment);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(44);
    field();
    setState(45);
    match(dzParser::T__4);
    setState(46);
    expression(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FieldContext ------------------------------------------------------------------

dzParser::FieldContext::FieldContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* dzParser::FieldContext::ID() {
  return getToken(dzParser::ID, 0);
}


size_t dzParser::FieldContext::getRuleIndex() const {
  return dzParser::RuleField;
}


antlrcpp::Any dzParser::FieldContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitField(this);
  else
    return visitor->visitChildren(this);
}

dzParser::FieldContext* dzParser::field() {
  FieldContext *_localctx = _tracker.createInstance<FieldContext>(_ctx, getState());
  enterRule(_localctx, 6, dzParser::RuleField);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(48);
    match(dzParser::ID);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionContext ------------------------------------------------------------------

dzParser::FunctionContext::FunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

dzParser::TypeNameContext* dzParser::FunctionContext::typeName() {
  return getRuleContext<dzParser::TypeNameContext>(0);
}

std::vector<tree::TerminalNode *> dzParser::FunctionContext::ID() {
  return getTokens(dzParser::ID);
}

tree::TerminalNode* dzParser::FunctionContext::ID(size_t i) {
  return getToken(dzParser::ID, i);
}

std::vector<dzParser::ArgumentContext *> dzParser::FunctionContext::argument() {
  return getRuleContexts<dzParser::ArgumentContext>();
}

dzParser::ArgumentContext* dzParser::FunctionContext::argument(size_t i) {
  return getRuleContext<dzParser::ArgumentContext>(i);
}

dzParser::BlockContext* dzParser::FunctionContext::block() {
  return getRuleContext<dzParser::BlockContext>(0);
}


size_t dzParser::FunctionContext::getRuleIndex() const {
  return dzParser::RuleFunction;
}


antlrcpp::Any dzParser::FunctionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitFunction(this);
  else
    return visitor->visitChildren(this);
}

dzParser::FunctionContext* dzParser::function() {
  FunctionContext *_localctx = _tracker.createInstance<FunctionContext>(_ctx, getState());
  enterRule(_localctx, 8, dzParser::RuleFunction);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(51);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      setState(50);
      dynamic_cast<FunctionContext *>(_localctx)->attribute = match(dzParser::ID);
      break;
    }

    default:
      break;
    }
    setState(53);
    typeName();
    setState(54);
    dynamic_cast<FunctionContext *>(_localctx)->name = match(dzParser::ID);
    setState(55);
    match(dzParser::T__5);
    setState(64);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == dzParser::ID) {
      setState(56);
      argument();
      setState(61);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == dzParser::T__2) {
        setState(57);
        match(dzParser::T__2);
        setState(58);
        argument();
        setState(63);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(66);
    match(dzParser::T__6);
    setState(68);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == dzParser::T__1) {
      setState(67);
      block();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LiteralContext ------------------------------------------------------------------

dzParser::LiteralContext::LiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t dzParser::LiteralContext::getRuleIndex() const {
  return dzParser::RuleLiteral;
}

void dzParser::LiteralContext::copyFrom(LiteralContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- StringLiteralContext ------------------------------------------------------------------

tree::TerminalNode* dzParser::StringLiteralContext::STRING() {
  return getToken(dzParser::STRING, 0);
}

dzParser::StringLiteralContext::StringLiteralContext(LiteralContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::StringLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitStringLiteral(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Int64LiteralContext ------------------------------------------------------------------

tree::TerminalNode* dzParser::Int64LiteralContext::INT() {
  return getToken(dzParser::INT, 0);
}

dzParser::Int64LiteralContext::Int64LiteralContext(LiteralContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::Int64LiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitInt64Literal(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BoolLiteralContext ------------------------------------------------------------------

tree::TerminalNode* dzParser::BoolLiteralContext::BOOL() {
  return getToken(dzParser::BOOL, 0);
}

dzParser::BoolLiteralContext::BoolLiteralContext(LiteralContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::BoolLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitBoolLiteral(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Int32LiteralContext ------------------------------------------------------------------

tree::TerminalNode* dzParser::Int32LiteralContext::INT() {
  return getToken(dzParser::INT, 0);
}

dzParser::Int32LiteralContext::Int32LiteralContext(LiteralContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::Int32LiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitInt32Literal(this);
  else
    return visitor->visitChildren(this);
}
dzParser::LiteralContext* dzParser::literal() {
  LiteralContext *_localctx = _tracker.createInstance<LiteralContext>(_ctx, getState());
  enterRule(_localctx, 10, dzParser::RuleLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(75);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx)) {
    case 1: {
      _localctx = dynamic_cast<LiteralContext *>(_tracker.createInstance<dzParser::Int32LiteralContext>(_localctx));
      enterOuterAlt(_localctx, 1);
      setState(70);
      match(dzParser::INT);
      break;
    }

    case 2: {
      _localctx = dynamic_cast<LiteralContext *>(_tracker.createInstance<dzParser::Int64LiteralContext>(_localctx));
      enterOuterAlt(_localctx, 2);
      setState(71);
      match(dzParser::INT);
      setState(72);
      match(dzParser::T__7);
      break;
    }

    case 3: {
      _localctx = dynamic_cast<LiteralContext *>(_tracker.createInstance<dzParser::BoolLiteralContext>(_localctx));
      enterOuterAlt(_localctx, 3);
      setState(73);
      match(dzParser::BOOL);
      break;
    }

    case 4: {
      _localctx = dynamic_cast<LiteralContext *>(_tracker.createInstance<dzParser::StringLiteralContext>(_localctx));
      enterOuterAlt(_localctx, 4);
      setState(74);
      match(dzParser::STRING);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

dzParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t dzParser::ExpressionContext::getRuleIndex() const {
  return dzParser::RuleExpression;
}

void dzParser::ExpressionContext::copyFrom(ExpressionContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- CallContext ------------------------------------------------------------------

tree::TerminalNode* dzParser::CallContext::ID() {
  return getToken(dzParser::ID, 0);
}

std::vector<dzParser::ExpressionContext *> dzParser::CallContext::expression() {
  return getRuleContexts<dzParser::ExpressionContext>();
}

dzParser::ExpressionContext* dzParser::CallContext::expression(size_t i) {
  return getRuleContext<dzParser::ExpressionContext>(i);
}

dzParser::CallContext::CallContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::CallContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitCall(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ConstantContext ------------------------------------------------------------------

dzParser::LiteralContext* dzParser::ConstantContext::literal() {
  return getRuleContext<dzParser::LiteralContext>(0);
}

dzParser::ConstantContext::ConstantContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::ConstantContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitConstant(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BinaryContext ------------------------------------------------------------------

std::vector<dzParser::ExpressionContext *> dzParser::BinaryContext::expression() {
  return getRuleContexts<dzParser::ExpressionContext>();
}

dzParser::ExpressionContext* dzParser::BinaryContext::expression(size_t i) {
  return getRuleContext<dzParser::ExpressionContext>(i);
}

tree::TerminalNode* dzParser::BinaryContext::OP() {
  return getToken(dzParser::OP, 0);
}

dzParser::BinaryContext::BinaryContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::BinaryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitBinary(this);
  else
    return visitor->visitChildren(this);
}
//----------------- MemberContext ------------------------------------------------------------------

std::vector<tree::TerminalNode *> dzParser::MemberContext::ID() {
  return getTokens(dzParser::ID);
}

tree::TerminalNode* dzParser::MemberContext::ID(size_t i) {
  return getToken(dzParser::ID, i);
}

dzParser::MemberContext::MemberContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::MemberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitMember(this);
  else
    return visitor->visitChildren(this);
}
//----------------- InstantiationContext ------------------------------------------------------------------

dzParser::TypeNameContext* dzParser::InstantiationContext::typeName() {
  return getRuleContext<dzParser::TypeNameContext>(0);
}

std::vector<dzParser::AssignmentContext *> dzParser::InstantiationContext::assignment() {
  return getRuleContexts<dzParser::AssignmentContext>();
}

dzParser::AssignmentContext* dzParser::InstantiationContext::assignment(size_t i) {
  return getRuleContext<dzParser::AssignmentContext>(i);
}

dzParser::InstantiationContext::InstantiationContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::InstantiationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitInstantiation(this);
  else
    return visitor->visitChildren(this);
}

dzParser::ExpressionContext* dzParser::expression() {
   return expression(0);
}

dzParser::ExpressionContext* dzParser::expression(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  dzParser::ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, parentState);
  dzParser::ExpressionContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 12;
  enterRecursionRule(_localctx, 12, dzParser::RuleExpression, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(112);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<CallContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(78);
      match(dzParser::ID);
      setState(79);
      match(dzParser::T__5);
      setState(88);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << dzParser::INT)
        | (1ULL << dzParser::BOOL)
        | (1ULL << dzParser::STRING)
        | (1ULL << dzParser::ID))) != 0)) {
        setState(80);
        expression(0);
        setState(85);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == dzParser::T__2) {
          setState(81);
          match(dzParser::T__2);
          setState(82);
          expression(0);
          setState(87);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(90);
      match(dzParser::T__6);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<ConstantContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(91);
      literal();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MemberContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(92);
      match(dzParser::ID);
      setState(97);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(93);
          match(dzParser::T__8);
          setState(94);
          match(dzParser::ID); 
        }
        setState(99);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx);
      }
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<InstantiationContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(100);
      typeName();
      setState(101);
      match(dzParser::T__1);
      setState(102);
      assignment();
      setState(107);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == dzParser::T__2) {
        setState(103);
        match(dzParser::T__2);
        setState(104);
        assignment();
        setState(109);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(110);
      match(dzParser::T__3);
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(119);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        auto newContext = _tracker.createInstance<BinaryContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
        _localctx = newContext;
        newContext->left = previousContext;
        pushNewRecursionContext(newContext, startState, RuleExpression);
        setState(114);

        if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
        setState(115);
        dynamic_cast<BinaryContext *>(_localctx)->op = match(dzParser::OP);
        setState(116);
        dynamic_cast<BinaryContext *>(_localctx)->right = expression(4); 
      }
      setState(121);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- RetContext ------------------------------------------------------------------

dzParser::RetContext::RetContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<dzParser::ExpressionContext *> dzParser::RetContext::expression() {
  return getRuleContexts<dzParser::ExpressionContext>();
}

dzParser::ExpressionContext* dzParser::RetContext::expression(size_t i) {
  return getRuleContext<dzParser::ExpressionContext>(i);
}


size_t dzParser::RetContext::getRuleIndex() const {
  return dzParser::RuleRet;
}


antlrcpp::Any dzParser::RetContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitRet(this);
  else
    return visitor->visitChildren(this);
}

dzParser::RetContext* dzParser::ret() {
  RetContext *_localctx = _tracker.createInstance<RetContext>(_ctx, getState());
  enterRule(_localctx, 14, dzParser::RuleRet);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(122);
    match(dzParser::T__9);
    setState(124);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << dzParser::INT)
      | (1ULL << dzParser::BOOL)
      | (1ULL << dzParser::STRING)
      | (1ULL << dzParser::ID))) != 0)) {
      setState(123);
      dynamic_cast<RetContext *>(_localctx)->value = expression(0);
    }
    setState(128);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == dzParser::T__10) {
      setState(126);
      match(dzParser::T__10);
      setState(127);
      dynamic_cast<RetContext *>(_localctx)->chained = expression(0);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConditionalContext ------------------------------------------------------------------

dzParser::ConditionalContext::ConditionalContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

dzParser::ExpressionContext* dzParser::ConditionalContext::expression() {
  return getRuleContext<dzParser::ExpressionContext>(0);
}

dzParser::BlockContext* dzParser::ConditionalContext::block() {
  return getRuleContext<dzParser::BlockContext>(0);
}


size_t dzParser::ConditionalContext::getRuleIndex() const {
  return dzParser::RuleConditional;
}


antlrcpp::Any dzParser::ConditionalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitConditional(this);
  else
    return visitor->visitChildren(this);
}

dzParser::ConditionalContext* dzParser::conditional() {
  ConditionalContext *_localctx = _tracker.createInstance<ConditionalContext>(_ctx, getState());
  enterRule(_localctx, 16, dzParser::RuleConditional);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(130);
    match(dzParser::T__11);
    setState(131);
    match(dzParser::T__5);
    setState(132);
    expression(0);
    setState(133);
    match(dzParser::T__6);
    setState(134);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

dzParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

dzParser::RetContext* dzParser::BlockContext::ret() {
  return getRuleContext<dzParser::RetContext>(0);
}

std::vector<dzParser::ConditionalContext *> dzParser::BlockContext::conditional() {
  return getRuleContexts<dzParser::ConditionalContext>();
}

dzParser::ConditionalContext* dzParser::BlockContext::conditional(size_t i) {
  return getRuleContext<dzParser::ConditionalContext>(i);
}


size_t dzParser::BlockContext::getRuleIndex() const {
  return dzParser::RuleBlock;
}


antlrcpp::Any dzParser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

dzParser::BlockContext* dzParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 18, dzParser::RuleBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(136);
    match(dzParser::T__1);
    setState(140);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == dzParser::T__11) {
      setState(137);
      conditional();
      setState(142);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(143);
    ret();
    setState(144);
    match(dzParser::T__3);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArgumentContext ------------------------------------------------------------------

dzParser::ArgumentContext::ArgumentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

dzParser::TypeNameContext* dzParser::ArgumentContext::typeName() {
  return getRuleContext<dzParser::TypeNameContext>(0);
}

tree::TerminalNode* dzParser::ArgumentContext::ID() {
  return getToken(dzParser::ID, 0);
}


size_t dzParser::ArgumentContext::getRuleIndex() const {
  return dzParser::RuleArgument;
}


antlrcpp::Any dzParser::ArgumentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitArgument(this);
  else
    return visitor->visitChildren(this);
}

dzParser::ArgumentContext* dzParser::argument() {
  ArgumentContext *_localctx = _tracker.createInstance<ArgumentContext>(_ctx, getState());
  enterRule(_localctx, 20, dzParser::RuleArgument);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(146);
    typeName();
    setState(147);
    match(dzParser::ID);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeNameContext ------------------------------------------------------------------

dzParser::TypeNameContext::TypeNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* dzParser::TypeNameContext::ID() {
  return getToken(dzParser::ID, 0);
}


size_t dzParser::TypeNameContext::getRuleIndex() const {
  return dzParser::RuleTypeName;
}


antlrcpp::Any dzParser::TypeNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitTypeName(this);
  else
    return visitor->visitChildren(this);
}

dzParser::TypeNameContext* dzParser::typeName() {
  TypeNameContext *_localctx = _tracker.createInstance<TypeNameContext>(_ctx, getState());
  enterRule(_localctx, 22, dzParser::RuleTypeName);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(149);
    match(dzParser::ID);
    setState(151);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == dzParser::T__12) {
      setState(150);
      match(dzParser::T__12);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool dzParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 6: return expressionSempred(dynamic_cast<ExpressionContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool dzParser::expressionSempred(ExpressionContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 3);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> dzParser::_decisionToDFA;
atn::PredictionContextCache dzParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN dzParser::_atn;
std::vector<uint16_t> dzParser::_serializedATN;

std::vector<std::string> dzParser::_ruleNames = {
  "program", "structure", "assignment", "field", "function", "literal", 
  "expression", "ret", "conditional", "block", "argument", "typeName"
};

std::vector<std::string> dzParser::_literalNames = {
  "", "'struct'", "'{'", "','", "'}'", "':'", "'('", "')'", "'L'", "'.'", 
  "'return'", "'->'", "'if'", "'...'"
};

std::vector<std::string> dzParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "INT", "BOOL", 
  "OP", "STRING", "WS", "ID"
};

dfa::Vocabulary dzParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> dzParser::_tokenNames;

dzParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  static const uint16_t serializedATNSegment0[] = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
       0x3, 0x15, 0x9c, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
       0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
       0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 
       0x4, 0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x3, 
       0x2, 0x3, 0x2, 0x7, 0x2, 0x1d, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 0x20, 
       0xb, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
       0x3, 0x7, 0x3, 0x28, 0xa, 0x3, 0xc, 0x3, 0xe, 0x3, 0x2b, 0xb, 0x3, 
       0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
       0x5, 0x3, 0x5, 0x3, 0x6, 0x5, 0x6, 0x36, 0xa, 0x6, 0x3, 0x6, 0x3, 
       0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x7, 0x6, 0x3e, 0xa, 
       0x6, 0xc, 0x6, 0xe, 0x6, 0x41, 0xb, 0x6, 0x5, 0x6, 0x43, 0xa, 0x6, 
       0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0x47, 0xa, 0x6, 0x3, 0x7, 0x3, 0x7, 
       0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x5, 0x7, 0x4e, 0xa, 0x7, 0x3, 0x8, 
       0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x7, 0x8, 0x56, 
       0xa, 0x8, 0xc, 0x8, 0xe, 0x8, 0x59, 0xb, 0x8, 0x5, 0x8, 0x5b, 0xa, 
       0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x7, 0x8, 
       0x62, 0xa, 0x8, 0xc, 0x8, 0xe, 0x8, 0x65, 0xb, 0x8, 0x3, 0x8, 0x3, 
       0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x7, 0x8, 0x6c, 0xa, 0x8, 0xc, 
       0x8, 0xe, 0x8, 0x6f, 0xb, 0x8, 0x3, 0x8, 0x3, 0x8, 0x5, 0x8, 0x73, 
       0xa, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x7, 0x8, 0x78, 0xa, 0x8, 
       0xc, 0x8, 0xe, 0x8, 0x7b, 0xb, 0x8, 0x3, 0x9, 0x3, 0x9, 0x5, 0x9, 
       0x7f, 0xa, 0x9, 0x3, 0x9, 0x3, 0x9, 0x5, 0x9, 0x83, 0xa, 0x9, 0x3, 
       0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xb, 
       0x3, 0xb, 0x7, 0xb, 0x8d, 0xa, 0xb, 0xc, 0xb, 0xe, 0xb, 0x90, 0xb, 
       0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 
       0x3, 0xd, 0x3, 0xd, 0x5, 0xd, 0x9a, 0xa, 0xd, 0x3, 0xd, 0x2, 0x3, 
       0xe, 0xe, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 
       0x18, 0x2, 0x2, 0x2, 0xa5, 0x2, 0x1e, 0x3, 0x2, 0x2, 0x2, 0x4, 0x21, 
       0x3, 0x2, 0x2, 0x2, 0x6, 0x2e, 0x3, 0x2, 0x2, 0x2, 0x8, 0x32, 0x3, 
       0x2, 0x2, 0x2, 0xa, 0x35, 0x3, 0x2, 0x2, 0x2, 0xc, 0x4d, 0x3, 0x2, 
       0x2, 0x2, 0xe, 0x72, 0x3, 0x2, 0x2, 0x2, 0x10, 0x7c, 0x3, 0x2, 0x2, 
       0x2, 0x12, 0x84, 0x3, 0x2, 0x2, 0x2, 0x14, 0x8a, 0x3, 0x2, 0x2, 0x2, 
       0x16, 0x94, 0x3, 0x2, 0x2, 0x2, 0x18, 0x97, 0x3, 0x2, 0x2, 0x2, 0x1a, 
       0x1d, 0x5, 0xa, 0x6, 0x2, 0x1b, 0x1d, 0x5, 0x4, 0x3, 0x2, 0x1c, 0x1a, 
       0x3, 0x2, 0x2, 0x2, 0x1c, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x1d, 0x20, 0x3, 
       0x2, 0x2, 0x2, 0x1e, 0x1c, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x1f, 0x3, 0x2, 
       0x2, 0x2, 0x1f, 0x3, 0x3, 0x2, 0x2, 0x2, 0x20, 0x1e, 0x3, 0x2, 0x2, 
       0x2, 0x21, 0x22, 0x7, 0x3, 0x2, 0x2, 0x22, 0x23, 0x7, 0x15, 0x2, 
       0x2, 0x23, 0x24, 0x7, 0x4, 0x2, 0x2, 0x24, 0x29, 0x5, 0x8, 0x5, 0x2, 
       0x25, 0x26, 0x7, 0x5, 0x2, 0x2, 0x26, 0x28, 0x5, 0x8, 0x5, 0x2, 0x27, 
       0x25, 0x3, 0x2, 0x2, 0x2, 0x28, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x29, 0x27, 
       0x3, 0x2, 0x2, 0x2, 0x29, 0x2a, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x2c, 0x3, 
       0x2, 0x2, 0x2, 0x2b, 0x29, 0x3, 0x2, 0x2, 0x2, 0x2c, 0x2d, 0x7, 0x6, 
       0x2, 0x2, 0x2d, 0x5, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x2f, 0x5, 0x8, 0x5, 
       0x2, 0x2f, 0x30, 0x7, 0x7, 0x2, 0x2, 0x30, 0x31, 0x5, 0xe, 0x8, 0x2, 
       0x31, 0x7, 0x3, 0x2, 0x2, 0x2, 0x32, 0x33, 0x7, 0x15, 0x2, 0x2, 0x33, 
       0x9, 0x3, 0x2, 0x2, 0x2, 0x34, 0x36, 0x7, 0x15, 0x2, 0x2, 0x35, 0x34, 
       0x3, 0x2, 0x2, 0x2, 0x35, 0x36, 0x3, 0x2, 0x2, 0x2, 0x36, 0x37, 0x3, 
       0x2, 0x2, 0x2, 0x37, 0x38, 0x5, 0x18, 0xd, 0x2, 0x38, 0x39, 0x7, 
       0x15, 0x2, 0x2, 0x39, 0x42, 0x7, 0x8, 0x2, 0x2, 0x3a, 0x3f, 0x5, 
       0x16, 0xc, 0x2, 0x3b, 0x3c, 0x7, 0x5, 0x2, 0x2, 0x3c, 0x3e, 0x5, 
       0x16, 0xc, 0x2, 0x3d, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x41, 0x3, 
       0x2, 0x2, 0x2, 0x3f, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x3f, 0x40, 0x3, 0x2, 
       0x2, 0x2, 0x40, 0x43, 0x3, 0x2, 0x2, 0x2, 0x41, 0x3f, 0x3, 0x2, 0x2, 
       0x2, 0x42, 0x3a, 0x3, 0x2, 0x2, 0x2, 0x42, 0x43, 0x3, 0x2, 0x2, 0x2, 
       0x43, 0x44, 0x3, 0x2, 0x2, 0x2, 0x44, 0x46, 0x7, 0x9, 0x2, 0x2, 0x45, 
       0x47, 0x5, 0x14, 0xb, 0x2, 0x46, 0x45, 0x3, 0x2, 0x2, 0x2, 0x46, 
       0x47, 0x3, 0x2, 0x2, 0x2, 0x47, 0xb, 0x3, 0x2, 0x2, 0x2, 0x48, 0x4e, 
       0x7, 0x10, 0x2, 0x2, 0x49, 0x4a, 0x7, 0x10, 0x2, 0x2, 0x4a, 0x4e, 
       0x7, 0xa, 0x2, 0x2, 0x4b, 0x4e, 0x7, 0x11, 0x2, 0x2, 0x4c, 0x4e, 
       0x7, 0x13, 0x2, 0x2, 0x4d, 0x48, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x49, 
       0x3, 0x2, 0x2, 0x2, 0x4d, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x4c, 0x3, 
       0x2, 0x2, 0x2, 0x4e, 0xd, 0x3, 0x2, 0x2, 0x2, 0x4f, 0x50, 0x8, 0x8, 
       0x1, 0x2, 0x50, 0x51, 0x7, 0x15, 0x2, 0x2, 0x51, 0x5a, 0x7, 0x8, 
       0x2, 0x2, 0x52, 0x57, 0x5, 0xe, 0x8, 0x2, 0x53, 0x54, 0x7, 0x5, 0x2, 
       0x2, 0x54, 0x56, 0x5, 0xe, 0x8, 0x2, 0x55, 0x53, 0x3, 0x2, 0x2, 0x2, 
       0x56, 0x59, 0x3, 0x2, 0x2, 0x2, 0x57, 0x55, 0x3, 0x2, 0x2, 0x2, 0x57, 
       0x58, 0x3, 0x2, 0x2, 0x2, 0x58, 0x5b, 0x3, 0x2, 0x2, 0x2, 0x59, 0x57, 
       0x3, 0x2, 0x2, 0x2, 0x5a, 0x52, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x5b, 0x3, 
       0x2, 0x2, 0x2, 0x5b, 0x5c, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x73, 0x7, 0x9, 
       0x2, 0x2, 0x5d, 0x73, 0x5, 0xc, 0x7, 0x2, 0x5e, 0x63, 0x7, 0x15, 
       0x2, 0x2, 0x5f, 0x60, 0x7, 0xb, 0x2, 0x2, 0x60, 0x62, 0x7, 0x15, 
       0x2, 0x2, 0x61, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x62, 0x65, 0x3, 0x2, 0x2, 
       0x2, 0x63, 0x61, 0x3, 0x2, 0x2, 0x2, 0x63, 0x64, 0x3, 0x2, 0x2, 0x2, 
       0x64, 0x73, 0x3, 0x2, 0x2, 0x2, 0x65, 0x63, 0x3, 0x2, 0x2, 0x2, 0x66, 
       0x67, 0x5, 0x18, 0xd, 0x2, 0x67, 0x68, 0x7, 0x4, 0x2, 0x2, 0x68, 
       0x6d, 0x5, 0x6, 0x4, 0x2, 0x69, 0x6a, 0x7, 0x5, 0x2, 0x2, 0x6a, 0x6c, 
       0x5, 0x6, 0x4, 0x2, 0x6b, 0x69, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x6f, 0x3, 
       0x2, 0x2, 0x2, 0x6d, 0x6b, 0x3, 0x2, 0x2, 0x2, 0x6d, 0x6e, 0x3, 0x2, 
       0x2, 0x2, 0x6e, 0x70, 0x3, 0x2, 0x2, 0x2, 0x6f, 0x6d, 0x3, 0x2, 0x2, 
       0x2, 0x70, 0x71, 0x7, 0x6, 0x2, 0x2, 0x71, 0x73, 0x3, 0x2, 0x2, 0x2, 
       0x72, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x72, 0x5d, 0x3, 0x2, 0x2, 0x2, 0x72, 
       0x5e, 0x3, 0x2, 0x2, 0x2, 0x72, 0x66, 0x3, 0x2, 0x2, 0x2, 0x73, 0x79, 
       0x3, 0x2, 0x2, 0x2, 0x74, 0x75, 0xc, 0x5, 0x2, 0x2, 0x75, 0x76, 0x7, 
       0x12, 0x2, 0x2, 0x76, 0x78, 0x5, 0xe, 0x8, 0x6, 0x77, 0x74, 0x3, 
       0x2, 0x2, 0x2, 0x78, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x79, 0x77, 0x3, 0x2, 
       0x2, 0x2, 0x79, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x7a, 0xf, 0x3, 0x2, 0x2, 
       0x2, 0x7b, 0x79, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x7e, 0x7, 0xc, 0x2, 0x2, 
       0x7d, 0x7f, 0x5, 0xe, 0x8, 0x2, 0x7e, 0x7d, 0x3, 0x2, 0x2, 0x2, 0x7e, 
       0x7f, 0x3, 0x2, 0x2, 0x2, 0x7f, 0x82, 0x3, 0x2, 0x2, 0x2, 0x80, 0x81, 
       0x7, 0xd, 0x2, 0x2, 0x81, 0x83, 0x5, 0xe, 0x8, 0x2, 0x82, 0x80, 0x3, 
       0x2, 0x2, 0x2, 0x82, 0x83, 0x3, 0x2, 0x2, 0x2, 0x83, 0x11, 0x3, 0x2, 
       0x2, 0x2, 0x84, 0x85, 0x7, 0xe, 0x2, 0x2, 0x85, 0x86, 0x7, 0x8, 0x2, 
       0x2, 0x86, 0x87, 0x5, 0xe, 0x8, 0x2, 0x87, 0x88, 0x7, 0x9, 0x2, 0x2, 
       0x88, 0x89, 0x5, 0x14, 0xb, 0x2, 0x89, 0x13, 0x3, 0x2, 0x2, 0x2, 
       0x8a, 0x8e, 0x7, 0x4, 0x2, 0x2, 0x8b, 0x8d, 0x5, 0x12, 0xa, 0x2, 
       0x8c, 0x8b, 0x3, 0x2, 0x2, 0x2, 0x8d, 0x90, 0x3, 0x2, 0x2, 0x2, 0x8e, 
       0x8c, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x8f, 0x3, 0x2, 0x2, 0x2, 0x8f, 0x91, 
       0x3, 0x2, 0x2, 0x2, 0x90, 0x8e, 0x3, 0x2, 0x2, 0x2, 0x91, 0x92, 0x5, 
       0x10, 0x9, 0x2, 0x92, 0x93, 0x7, 0x6, 0x2, 0x2, 0x93, 0x15, 0x3, 
       0x2, 0x2, 0x2, 0x94, 0x95, 0x5, 0x18, 0xd, 0x2, 0x95, 0x96, 0x7, 
       0x15, 0x2, 0x2, 0x96, 0x17, 0x3, 0x2, 0x2, 0x2, 0x97, 0x99, 0x7, 
       0x15, 0x2, 0x2, 0x98, 0x9a, 0x7, 0xf, 0x2, 0x2, 0x99, 0x98, 0x3, 
       0x2, 0x2, 0x2, 0x99, 0x9a, 0x3, 0x2, 0x2, 0x2, 0x9a, 0x19, 0x3, 0x2, 
       0x2, 0x2, 0x14, 0x1c, 0x1e, 0x29, 0x35, 0x3f, 0x42, 0x46, 0x4d, 0x57, 
       0x5a, 0x63, 0x6d, 0x72, 0x79, 0x7e, 0x82, 0x8e, 0x99, 
  };

  _serializedATN.insert(_serializedATN.end(), serializedATNSegment0,
    serializedATNSegment0 + sizeof(serializedATNSegment0) / sizeof(serializedATNSegment0[0]));


  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

dzParser::Initializer dzParser::_init;
