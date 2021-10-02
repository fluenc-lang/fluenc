
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

std::vector<dzParser::GlobalContext *> dzParser::ProgramContext::global() {
  return getRuleContexts<dzParser::GlobalContext>();
}

dzParser::GlobalContext* dzParser::ProgramContext::global(size_t i) {
  return getRuleContext<dzParser::GlobalContext>(i);
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
    setState(29);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << dzParser::T__0)
      | (1ULL << dzParser::T__4)
      | (1ULL << dzParser::ID))) != 0)) {
      setState(27);
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

        case dzParser::T__4: {
          setState(26);
          global();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(31);
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
    setState(32);
    match(dzParser::T__0);
    setState(33);
    match(dzParser::ID);
    setState(34);
    match(dzParser::T__1);
    setState(35);
    field();
    setState(40);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == dzParser::T__2) {
      setState(36);
      match(dzParser::T__2);
      setState(37);
      field();
      setState(42);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(43);
    match(dzParser::T__3);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GlobalContext ------------------------------------------------------------------

dzParser::GlobalContext::GlobalContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* dzParser::GlobalContext::ID() {
  return getToken(dzParser::ID, 0);
}

dzParser::LiteralContext* dzParser::GlobalContext::literal() {
  return getRuleContext<dzParser::LiteralContext>(0);
}


size_t dzParser::GlobalContext::getRuleIndex() const {
  return dzParser::RuleGlobal;
}


antlrcpp::Any dzParser::GlobalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitGlobal(this);
  else
    return visitor->visitChildren(this);
}

dzParser::GlobalContext* dzParser::global() {
  GlobalContext *_localctx = _tracker.createInstance<GlobalContext>(_ctx, getState());
  enterRule(_localctx, 4, dzParser::RuleGlobal);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(45);
    match(dzParser::T__4);
    setState(46);
    match(dzParser::ID);
    setState(47);
    match(dzParser::T__5);
    setState(48);
    literal();
   
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
  enterRule(_localctx, 6, dzParser::RuleAssignment);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(50);
    field();
    setState(51);
    match(dzParser::T__5);
    setState(52);
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

dzParser::LiteralContext* dzParser::FieldContext::literal() {
  return getRuleContext<dzParser::LiteralContext>(0);
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
  enterRule(_localctx, 8, dzParser::RuleField);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(54);
    match(dzParser::ID);
    setState(57);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      setState(55);
      match(dzParser::T__5);
      setState(56);
      literal();
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
  enterRule(_localctx, 10, dzParser::RuleFunction);
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
    setState(60);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
    case 1: {
      setState(59);
      dynamic_cast<FunctionContext *>(_localctx)->attribute = match(dzParser::ID);
      break;
    }

    default:
      break;
    }
    setState(62);
    typeName();
    setState(63);
    dynamic_cast<FunctionContext *>(_localctx)->name = match(dzParser::ID);
    setState(64);
    match(dzParser::T__6);
    setState(73);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == dzParser::ID) {
      setState(65);
      argument();
      setState(70);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == dzParser::T__2) {
        setState(66);
        match(dzParser::T__2);
        setState(67);
        argument();
        setState(72);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(75);
    match(dzParser::T__7);
    setState(77);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == dzParser::T__1) {
      setState(76);
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
//----------------- Uint32LiteralContext ------------------------------------------------------------------

tree::TerminalNode* dzParser::Uint32LiteralContext::INT() {
  return getToken(dzParser::INT, 0);
}

dzParser::Uint32LiteralContext::Uint32LiteralContext(LiteralContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::Uint32LiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitUint32Literal(this);
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
  enterRule(_localctx, 12, dzParser::RuleLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(86);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx)) {
    case 1: {
      _localctx = dynamic_cast<LiteralContext *>(_tracker.createInstance<dzParser::Int32LiteralContext>(_localctx));
      enterOuterAlt(_localctx, 1);
      setState(79);
      match(dzParser::INT);
      break;
    }

    case 2: {
      _localctx = dynamic_cast<LiteralContext *>(_tracker.createInstance<dzParser::Int64LiteralContext>(_localctx));
      enterOuterAlt(_localctx, 2);
      setState(80);
      match(dzParser::INT);
      setState(81);
      match(dzParser::T__8);
      break;
    }

    case 3: {
      _localctx = dynamic_cast<LiteralContext *>(_tracker.createInstance<dzParser::BoolLiteralContext>(_localctx));
      enterOuterAlt(_localctx, 3);
      setState(82);
      match(dzParser::BOOL);
      break;
    }

    case 4: {
      _localctx = dynamic_cast<LiteralContext *>(_tracker.createInstance<dzParser::StringLiteralContext>(_localctx));
      enterOuterAlt(_localctx, 4);
      setState(83);
      match(dzParser::STRING);
      break;
    }

    case 5: {
      _localctx = dynamic_cast<LiteralContext *>(_tracker.createInstance<dzParser::Uint32LiteralContext>(_localctx));
      enterOuterAlt(_localctx, 5);
      setState(84);
      match(dzParser::INT);
      setState(85);
      match(dzParser::T__9);
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
//----------------- ConditionalContext ------------------------------------------------------------------

dzParser::ExpressionContext* dzParser::ConditionalContext::expression() {
  return getRuleContext<dzParser::ExpressionContext>(0);
}

dzParser::BlockContext* dzParser::ConditionalContext::block() {
  return getRuleContext<dzParser::BlockContext>(0);
}

dzParser::ConditionalContext::ConditionalContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::ConditionalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitConditional(this);
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
  size_t startState = 14;
  enterRecursionRule(_localctx, 14, dzParser::RuleExpression, precedence);

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
    setState(131);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<CallContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(89);
      match(dzParser::ID);
      setState(90);
      match(dzParser::T__6);
      setState(99);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << dzParser::T__11)
        | (1ULL << dzParser::INT)
        | (1ULL << dzParser::BOOL)
        | (1ULL << dzParser::STRING)
        | (1ULL << dzParser::ID))) != 0)) {
        setState(91);
        expression(0);
        setState(96);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == dzParser::T__2) {
          setState(92);
          match(dzParser::T__2);
          setState(93);
          expression(0);
          setState(98);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(101);
      match(dzParser::T__7);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<ConstantContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(102);
      literal();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MemberContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(103);
      match(dzParser::ID);
      setState(108);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(104);
          match(dzParser::T__10);
          setState(105);
          match(dzParser::ID); 
        }
        setState(110);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx);
      }
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<InstantiationContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(111);
      typeName();
      setState(112);
      match(dzParser::T__1);
      setState(114);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == dzParser::ID) {
        setState(113);
        assignment();
      }
      setState(120);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == dzParser::T__2) {
        setState(116);
        match(dzParser::T__2);
        setState(117);
        assignment();
        setState(122);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(123);
      match(dzParser::T__3);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<ConditionalContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(125);
      match(dzParser::T__11);
      setState(126);
      match(dzParser::T__6);
      setState(127);
      expression(0);
      setState(128);
      match(dzParser::T__7);
      setState(129);
      block();
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(138);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        auto newContext = _tracker.createInstance<BinaryContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
        _localctx = newContext;
        newContext->left = previousContext;
        pushNewRecursionContext(newContext, startState, RuleExpression);
        setState(133);

        if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
        setState(134);
        dynamic_cast<BinaryContext *>(_localctx)->op = match(dzParser::OP);
        setState(135);
        dynamic_cast<BinaryContext *>(_localctx)->right = expression(5); 
      }
      setState(140);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx);
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
  enterRule(_localctx, 16, dzParser::RuleRet);
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
    setState(141);
    match(dzParser::T__12);
    setState(143);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << dzParser::T__11)
      | (1ULL << dzParser::INT)
      | (1ULL << dzParser::BOOL)
      | (1ULL << dzParser::STRING)
      | (1ULL << dzParser::ID))) != 0)) {
      setState(142);
      dynamic_cast<RetContext *>(_localctx)->value = expression(0);
    }
    setState(147);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == dzParser::T__13) {
      setState(145);
      match(dzParser::T__13);
      setState(146);
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

//----------------- BlockContext ------------------------------------------------------------------

dzParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

dzParser::RetContext* dzParser::BlockContext::ret() {
  return getRuleContext<dzParser::RetContext>(0);
}

std::vector<dzParser::ExpressionContext *> dzParser::BlockContext::expression() {
  return getRuleContexts<dzParser::ExpressionContext>();
}

dzParser::ExpressionContext* dzParser::BlockContext::expression(size_t i) {
  return getRuleContext<dzParser::ExpressionContext>(i);
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
    setState(149);
    match(dzParser::T__1);
    setState(153);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << dzParser::T__11)
      | (1ULL << dzParser::INT)
      | (1ULL << dzParser::BOOL)
      | (1ULL << dzParser::STRING)
      | (1ULL << dzParser::ID))) != 0)) {
      setState(150);
      expression(0);
      setState(155);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(156);
    ret();
    setState(157);
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
    setState(159);
    typeName();
    setState(160);
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
    setState(162);
    match(dzParser::ID);
    setState(164);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == dzParser::T__14) {
      setState(163);
      match(dzParser::T__14);
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
    case 7: return expressionSempred(dynamic_cast<ExpressionContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool dzParser::expressionSempred(ExpressionContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 4);

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
  "program", "structure", "global", "assignment", "field", "function", "literal", 
  "expression", "ret", "block", "argument", "typeName"
};

std::vector<std::string> dzParser::_literalNames = {
  "", "'struct'", "'{'", "','", "'}'", "'global'", "':'", "'('", "')'", 
  "'L'", "'u'", "'.'", "'if'", "'return'", "'->'", "'...'"
};

std::vector<std::string> dzParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "INT", 
  "BOOL", "OP", "STRING", "WS", "ID"
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
       0x3, 0x17, 0xa9, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
       0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
       0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 
       0x4, 0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x3, 
       0x2, 0x3, 0x2, 0x3, 0x2, 0x7, 0x2, 0x1e, 0xa, 0x2, 0xc, 0x2, 0xe, 
       0x2, 0x21, 0xb, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
       0x3, 0x3, 0x3, 0x7, 0x3, 0x29, 0xa, 0x3, 0xc, 0x3, 0xe, 0x3, 0x2c, 
       0xb, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
       0x4, 0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 
       0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0x3c, 0xa, 0x6, 0x3, 0x7, 0x5, 0x7, 
       0x3f, 0xa, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 
       0x3, 0x7, 0x7, 0x7, 0x47, 0xa, 0x7, 0xc, 0x7, 0xe, 0x7, 0x4a, 0xb, 
       0x7, 0x5, 0x7, 0x4c, 0xa, 0x7, 0x3, 0x7, 0x3, 0x7, 0x5, 0x7, 0x50, 
       0xa, 0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 
       0x8, 0x3, 0x8, 0x5, 0x8, 0x59, 0xa, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 
       0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x7, 0x9, 0x61, 0xa, 0x9, 0xc, 
       0x9, 0xe, 0x9, 0x64, 0xb, 0x9, 0x5, 0x9, 0x66, 0xa, 0x9, 0x3, 0x9, 
       0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x7, 0x9, 0x6d, 0xa, 0x9, 
       0xc, 0x9, 0xe, 0x9, 0x70, 0xb, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 
       0x5, 0x9, 0x75, 0xa, 0x9, 0x3, 0x9, 0x3, 0x9, 0x7, 0x9, 0x79, 0xa, 
       0x9, 0xc, 0x9, 0xe, 0x9, 0x7c, 0xb, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 
       0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x5, 0x9, 
       0x86, 0xa, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x7, 0x9, 0x8b, 0xa, 
       0x9, 0xc, 0x9, 0xe, 0x9, 0x8e, 0xb, 0x9, 0x3, 0xa, 0x3, 0xa, 0x5, 
       0xa, 0x92, 0xa, 0xa, 0x3, 0xa, 0x3, 0xa, 0x5, 0xa, 0x96, 0xa, 0xa, 
       0x3, 0xb, 0x3, 0xb, 0x7, 0xb, 0x9a, 0xa, 0xb, 0xc, 0xb, 0xe, 0xb, 
       0x9d, 0xb, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 
       0x3, 0xc, 0x3, 0xd, 0x3, 0xd, 0x5, 0xd, 0xa7, 0xa, 0xd, 0x3, 0xd, 
       0x2, 0x3, 0x10, 0xe, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 
       0x14, 0x16, 0x18, 0x2, 0x2, 0x2, 0xb7, 0x2, 0x1f, 0x3, 0x2, 0x2, 
       0x2, 0x4, 0x22, 0x3, 0x2, 0x2, 0x2, 0x6, 0x2f, 0x3, 0x2, 0x2, 0x2, 
       0x8, 0x34, 0x3, 0x2, 0x2, 0x2, 0xa, 0x38, 0x3, 0x2, 0x2, 0x2, 0xc, 
       0x3e, 0x3, 0x2, 0x2, 0x2, 0xe, 0x58, 0x3, 0x2, 0x2, 0x2, 0x10, 0x85, 
       0x3, 0x2, 0x2, 0x2, 0x12, 0x8f, 0x3, 0x2, 0x2, 0x2, 0x14, 0x97, 0x3, 
       0x2, 0x2, 0x2, 0x16, 0xa1, 0x3, 0x2, 0x2, 0x2, 0x18, 0xa4, 0x3, 0x2, 
       0x2, 0x2, 0x1a, 0x1e, 0x5, 0xc, 0x7, 0x2, 0x1b, 0x1e, 0x5, 0x4, 0x3, 
       0x2, 0x1c, 0x1e, 0x5, 0x6, 0x4, 0x2, 0x1d, 0x1a, 0x3, 0x2, 0x2, 0x2, 
       0x1d, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x1d, 0x1c, 0x3, 0x2, 0x2, 0x2, 0x1e, 
       0x21, 0x3, 0x2, 0x2, 0x2, 0x1f, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x1f, 0x20, 
       0x3, 0x2, 0x2, 0x2, 0x20, 0x3, 0x3, 0x2, 0x2, 0x2, 0x21, 0x1f, 0x3, 
       0x2, 0x2, 0x2, 0x22, 0x23, 0x7, 0x3, 0x2, 0x2, 0x23, 0x24, 0x7, 0x17, 
       0x2, 0x2, 0x24, 0x25, 0x7, 0x4, 0x2, 0x2, 0x25, 0x2a, 0x5, 0xa, 0x6, 
       0x2, 0x26, 0x27, 0x7, 0x5, 0x2, 0x2, 0x27, 0x29, 0x5, 0xa, 0x6, 0x2, 
       0x28, 0x26, 0x3, 0x2, 0x2, 0x2, 0x29, 0x2c, 0x3, 0x2, 0x2, 0x2, 0x2a, 
       0x28, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x2b, 0x2d, 
       0x3, 0x2, 0x2, 0x2, 0x2c, 0x2a, 0x3, 0x2, 0x2, 0x2, 0x2d, 0x2e, 0x7, 
       0x6, 0x2, 0x2, 0x2e, 0x5, 0x3, 0x2, 0x2, 0x2, 0x2f, 0x30, 0x7, 0x7, 
       0x2, 0x2, 0x30, 0x31, 0x7, 0x17, 0x2, 0x2, 0x31, 0x32, 0x7, 0x8, 
       0x2, 0x2, 0x32, 0x33, 0x5, 0xe, 0x8, 0x2, 0x33, 0x7, 0x3, 0x2, 0x2, 
       0x2, 0x34, 0x35, 0x5, 0xa, 0x6, 0x2, 0x35, 0x36, 0x7, 0x8, 0x2, 0x2, 
       0x36, 0x37, 0x5, 0x10, 0x9, 0x2, 0x37, 0x9, 0x3, 0x2, 0x2, 0x2, 0x38, 
       0x3b, 0x7, 0x17, 0x2, 0x2, 0x39, 0x3a, 0x7, 0x8, 0x2, 0x2, 0x3a, 
       0x3c, 0x5, 0xe, 0x8, 0x2, 0x3b, 0x39, 0x3, 0x2, 0x2, 0x2, 0x3b, 0x3c, 
       0x3, 0x2, 0x2, 0x2, 0x3c, 0xb, 0x3, 0x2, 0x2, 0x2, 0x3d, 0x3f, 0x7, 
       0x17, 0x2, 0x2, 0x3e, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x3f, 0x3, 
       0x2, 0x2, 0x2, 0x3f, 0x40, 0x3, 0x2, 0x2, 0x2, 0x40, 0x41, 0x5, 0x18, 
       0xd, 0x2, 0x41, 0x42, 0x7, 0x17, 0x2, 0x2, 0x42, 0x4b, 0x7, 0x9, 
       0x2, 0x2, 0x43, 0x48, 0x5, 0x16, 0xc, 0x2, 0x44, 0x45, 0x7, 0x5, 
       0x2, 0x2, 0x45, 0x47, 0x5, 0x16, 0xc, 0x2, 0x46, 0x44, 0x3, 0x2, 
       0x2, 0x2, 0x47, 0x4a, 0x3, 0x2, 0x2, 0x2, 0x48, 0x46, 0x3, 0x2, 0x2, 
       0x2, 0x48, 0x49, 0x3, 0x2, 0x2, 0x2, 0x49, 0x4c, 0x3, 0x2, 0x2, 0x2, 
       0x4a, 0x48, 0x3, 0x2, 0x2, 0x2, 0x4b, 0x43, 0x3, 0x2, 0x2, 0x2, 0x4b, 
       0x4c, 0x3, 0x2, 0x2, 0x2, 0x4c, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x4f, 
       0x7, 0xa, 0x2, 0x2, 0x4e, 0x50, 0x5, 0x14, 0xb, 0x2, 0x4f, 0x4e, 
       0x3, 0x2, 0x2, 0x2, 0x4f, 0x50, 0x3, 0x2, 0x2, 0x2, 0x50, 0xd, 0x3, 
       0x2, 0x2, 0x2, 0x51, 0x59, 0x7, 0x12, 0x2, 0x2, 0x52, 0x53, 0x7, 
       0x12, 0x2, 0x2, 0x53, 0x59, 0x7, 0xb, 0x2, 0x2, 0x54, 0x59, 0x7, 
       0x13, 0x2, 0x2, 0x55, 0x59, 0x7, 0x15, 0x2, 0x2, 0x56, 0x57, 0x7, 
       0x12, 0x2, 0x2, 0x57, 0x59, 0x7, 0xc, 0x2, 0x2, 0x58, 0x51, 0x3, 
       0x2, 0x2, 0x2, 0x58, 0x52, 0x3, 0x2, 0x2, 0x2, 0x58, 0x54, 0x3, 0x2, 
       0x2, 0x2, 0x58, 0x55, 0x3, 0x2, 0x2, 0x2, 0x58, 0x56, 0x3, 0x2, 0x2, 
       0x2, 0x59, 0xf, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x5b, 0x8, 0x9, 0x1, 0x2, 
       0x5b, 0x5c, 0x7, 0x17, 0x2, 0x2, 0x5c, 0x65, 0x7, 0x9, 0x2, 0x2, 
       0x5d, 0x62, 0x5, 0x10, 0x9, 0x2, 0x5e, 0x5f, 0x7, 0x5, 0x2, 0x2, 
       0x5f, 0x61, 0x5, 0x10, 0x9, 0x2, 0x60, 0x5e, 0x3, 0x2, 0x2, 0x2, 
       0x61, 0x64, 0x3, 0x2, 0x2, 0x2, 0x62, 0x60, 0x3, 0x2, 0x2, 0x2, 0x62, 
       0x63, 0x3, 0x2, 0x2, 0x2, 0x63, 0x66, 0x3, 0x2, 0x2, 0x2, 0x64, 0x62, 
       0x3, 0x2, 0x2, 0x2, 0x65, 0x5d, 0x3, 0x2, 0x2, 0x2, 0x65, 0x66, 0x3, 
       0x2, 0x2, 0x2, 0x66, 0x67, 0x3, 0x2, 0x2, 0x2, 0x67, 0x86, 0x7, 0xa, 
       0x2, 0x2, 0x68, 0x86, 0x5, 0xe, 0x8, 0x2, 0x69, 0x6e, 0x7, 0x17, 
       0x2, 0x2, 0x6a, 0x6b, 0x7, 0xd, 0x2, 0x2, 0x6b, 0x6d, 0x7, 0x17, 
       0x2, 0x2, 0x6c, 0x6a, 0x3, 0x2, 0x2, 0x2, 0x6d, 0x70, 0x3, 0x2, 0x2, 
       0x2, 0x6e, 0x6c, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x6f, 0x3, 0x2, 0x2, 0x2, 
       0x6f, 0x86, 0x3, 0x2, 0x2, 0x2, 0x70, 0x6e, 0x3, 0x2, 0x2, 0x2, 0x71, 
       0x72, 0x5, 0x18, 0xd, 0x2, 0x72, 0x74, 0x7, 0x4, 0x2, 0x2, 0x73, 
       0x75, 0x5, 0x8, 0x5, 0x2, 0x74, 0x73, 0x3, 0x2, 0x2, 0x2, 0x74, 0x75, 
       0x3, 0x2, 0x2, 0x2, 0x75, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x76, 0x77, 0x7, 
       0x5, 0x2, 0x2, 0x77, 0x79, 0x5, 0x8, 0x5, 0x2, 0x78, 0x76, 0x3, 0x2, 
       0x2, 0x2, 0x79, 0x7c, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x78, 0x3, 0x2, 0x2, 
       0x2, 0x7a, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x7b, 0x7d, 0x3, 0x2, 0x2, 0x2, 
       0x7c, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x7d, 0x7e, 0x7, 0x6, 0x2, 0x2, 0x7e, 
       0x86, 0x3, 0x2, 0x2, 0x2, 0x7f, 0x80, 0x7, 0xe, 0x2, 0x2, 0x80, 0x81, 
       0x7, 0x9, 0x2, 0x2, 0x81, 0x82, 0x5, 0x10, 0x9, 0x2, 0x82, 0x83, 
       0x7, 0xa, 0x2, 0x2, 0x83, 0x84, 0x5, 0x14, 0xb, 0x2, 0x84, 0x86, 
       0x3, 0x2, 0x2, 0x2, 0x85, 0x5a, 0x3, 0x2, 0x2, 0x2, 0x85, 0x68, 0x3, 
       0x2, 0x2, 0x2, 0x85, 0x69, 0x3, 0x2, 0x2, 0x2, 0x85, 0x71, 0x3, 0x2, 
       0x2, 0x2, 0x85, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x86, 0x8c, 0x3, 0x2, 0x2, 
       0x2, 0x87, 0x88, 0xc, 0x6, 0x2, 0x2, 0x88, 0x89, 0x7, 0x14, 0x2, 
       0x2, 0x89, 0x8b, 0x5, 0x10, 0x9, 0x7, 0x8a, 0x87, 0x3, 0x2, 0x2, 
       0x2, 0x8b, 0x8e, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x8a, 0x3, 0x2, 0x2, 0x2, 
       0x8c, 0x8d, 0x3, 0x2, 0x2, 0x2, 0x8d, 0x11, 0x3, 0x2, 0x2, 0x2, 0x8e, 
       0x8c, 0x3, 0x2, 0x2, 0x2, 0x8f, 0x91, 0x7, 0xf, 0x2, 0x2, 0x90, 0x92, 
       0x5, 0x10, 0x9, 0x2, 0x91, 0x90, 0x3, 0x2, 0x2, 0x2, 0x91, 0x92, 
       0x3, 0x2, 0x2, 0x2, 0x92, 0x95, 0x3, 0x2, 0x2, 0x2, 0x93, 0x94, 0x7, 
       0x10, 0x2, 0x2, 0x94, 0x96, 0x5, 0x10, 0x9, 0x2, 0x95, 0x93, 0x3, 
       0x2, 0x2, 0x2, 0x95, 0x96, 0x3, 0x2, 0x2, 0x2, 0x96, 0x13, 0x3, 0x2, 
       0x2, 0x2, 0x97, 0x9b, 0x7, 0x4, 0x2, 0x2, 0x98, 0x9a, 0x5, 0x10, 
       0x9, 0x2, 0x99, 0x98, 0x3, 0x2, 0x2, 0x2, 0x9a, 0x9d, 0x3, 0x2, 0x2, 
       0x2, 0x9b, 0x99, 0x3, 0x2, 0x2, 0x2, 0x9b, 0x9c, 0x3, 0x2, 0x2, 0x2, 
       0x9c, 0x9e, 0x3, 0x2, 0x2, 0x2, 0x9d, 0x9b, 0x3, 0x2, 0x2, 0x2, 0x9e, 
       0x9f, 0x5, 0x12, 0xa, 0x2, 0x9f, 0xa0, 0x7, 0x6, 0x2, 0x2, 0xa0, 
       0x15, 0x3, 0x2, 0x2, 0x2, 0xa1, 0xa2, 0x5, 0x18, 0xd, 0x2, 0xa2, 
       0xa3, 0x7, 0x17, 0x2, 0x2, 0xa3, 0x17, 0x3, 0x2, 0x2, 0x2, 0xa4, 
       0xa6, 0x7, 0x17, 0x2, 0x2, 0xa5, 0xa7, 0x7, 0x11, 0x2, 0x2, 0xa6, 
       0xa5, 0x3, 0x2, 0x2, 0x2, 0xa6, 0xa7, 0x3, 0x2, 0x2, 0x2, 0xa7, 0x19, 
       0x3, 0x2, 0x2, 0x2, 0x16, 0x1d, 0x1f, 0x2a, 0x3b, 0x3e, 0x48, 0x4b, 
       0x4f, 0x58, 0x62, 0x65, 0x6e, 0x74, 0x7a, 0x85, 0x8c, 0x91, 0x95, 
       0x9b, 0xa6, 
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
