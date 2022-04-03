
// Generated from dz.g4 by ANTLR 4.9.3


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

std::vector<dzParser::InstructionContext *> dzParser::ProgramContext::instruction() {
  return getRuleContexts<dzParser::InstructionContext>();
}

dzParser::InstructionContext* dzParser::ProgramContext::instruction(size_t i) {
  return getRuleContext<dzParser::InstructionContext>(i);
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
    setState(35);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << dzParser::T__0)
      | (1ULL << dzParser::T__3)
      | (1ULL << dzParser::T__6)
      | (1ULL << dzParser::T__7)
      | (1ULL << dzParser::T__10)
      | (1ULL << dzParser::T__11)
      | (1ULL << dzParser::COMMENT))) != 0)) {
      setState(32);
      instruction();
      setState(37);
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

//----------------- InstructionContext ------------------------------------------------------------------

dzParser::InstructionContext::InstructionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* dzParser::InstructionContext::COMMENT() {
  return getToken(dzParser::COMMENT, 0);
}

dzParser::FunctionContext* dzParser::InstructionContext::function() {
  return getRuleContext<dzParser::FunctionContext>(0);
}

dzParser::StructureContext* dzParser::InstructionContext::structure() {
  return getRuleContext<dzParser::StructureContext>(0);
}

dzParser::GlobalContext* dzParser::InstructionContext::global() {
  return getRuleContext<dzParser::GlobalContext>(0);
}

dzParser::NsContext* dzParser::InstructionContext::ns() {
  return getRuleContext<dzParser::NsContext>(0);
}


size_t dzParser::InstructionContext::getRuleIndex() const {
  return dzParser::RuleInstruction;
}


antlrcpp::Any dzParser::InstructionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitInstruction(this);
  else
    return visitor->visitChildren(this);
}

dzParser::InstructionContext* dzParser::instruction() {
  InstructionContext *_localctx = _tracker.createInstance<InstructionContext>(_ctx, getState());
  enterRule(_localctx, 2, dzParser::RuleInstruction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(43);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case dzParser::COMMENT: {
        setState(38);
        match(dzParser::COMMENT);
        break;
      }

      case dzParser::T__7:
      case dzParser::T__10:
      case dzParser::T__11: {
        setState(39);
        function();
        break;
      }

      case dzParser::T__3: {
        setState(40);
        structure();
        break;
      }

      case dzParser::T__6: {
        setState(41);
        global();
        break;
      }

      case dzParser::T__0: {
        setState(42);
        ns();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NsContext ------------------------------------------------------------------

dzParser::NsContext::NsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* dzParser::NsContext::ID() {
  return getToken(dzParser::ID, 0);
}

std::vector<dzParser::InstructionContext *> dzParser::NsContext::instruction() {
  return getRuleContexts<dzParser::InstructionContext>();
}

dzParser::InstructionContext* dzParser::NsContext::instruction(size_t i) {
  return getRuleContext<dzParser::InstructionContext>(i);
}


size_t dzParser::NsContext::getRuleIndex() const {
  return dzParser::RuleNs;
}


antlrcpp::Any dzParser::NsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitNs(this);
  else
    return visitor->visitChildren(this);
}

dzParser::NsContext* dzParser::ns() {
  NsContext *_localctx = _tracker.createInstance<NsContext>(_ctx, getState());
  enterRule(_localctx, 4, dzParser::RuleNs);
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
    setState(45);
    match(dzParser::T__0);
    setState(46);
    match(dzParser::ID);
    setState(47);
    match(dzParser::T__1);
    setState(51);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << dzParser::T__0)
      | (1ULL << dzParser::T__3)
      | (1ULL << dzParser::T__6)
      | (1ULL << dzParser::T__7)
      | (1ULL << dzParser::T__10)
      | (1ULL << dzParser::T__11)
      | (1ULL << dzParser::COMMENT))) != 0)) {
      setState(48);
      instruction();
      setState(53);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(54);
    match(dzParser::T__2);
   
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

std::vector<dzParser::FieldContext *> dzParser::StructureContext::field() {
  return getRuleContexts<dzParser::FieldContext>();
}

dzParser::FieldContext* dzParser::StructureContext::field(size_t i) {
  return getRuleContext<dzParser::FieldContext>(i);
}

tree::TerminalNode* dzParser::StructureContext::ID() {
  return getToken(dzParser::ID, 0);
}

std::vector<dzParser::TypeNameContext *> dzParser::StructureContext::typeName() {
  return getRuleContexts<dzParser::TypeNameContext>();
}

dzParser::TypeNameContext* dzParser::StructureContext::typeName(size_t i) {
  return getRuleContext<dzParser::TypeNameContext>(i);
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
  enterRule(_localctx, 6, dzParser::RuleStructure);
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
    setState(56);
    match(dzParser::T__3);
    setState(57);
    antlrcpp::downCast<StructureContext *>(_localctx)->name = match(dzParser::ID);
    setState(67);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == dzParser::T__4) {
      setState(58);
      match(dzParser::T__4);
      setState(59);
      typeName();
      setState(64);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == dzParser::T__5) {
        setState(60);
        match(dzParser::T__5);
        setState(61);
        typeName();
        setState(66);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(69);
    match(dzParser::T__1);
    setState(70);
    field();
    setState(75);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == dzParser::T__5) {
      setState(71);
      match(dzParser::T__5);
      setState(72);
      field();
      setState(77);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(78);
    match(dzParser::T__2);
   
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
  enterRule(_localctx, 8, dzParser::RuleGlobal);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(80);
    match(dzParser::T__6);
    setState(81);
    match(dzParser::ID);
    setState(82);
    match(dzParser::T__4);
    setState(83);
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
  enterRule(_localctx, 10, dzParser::RuleAssignment);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(85);
    field();
    setState(86);
    match(dzParser::T__4);
    setState(87);
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

dzParser::TypeNameContext* dzParser::FieldContext::typeName() {
  return getRuleContext<dzParser::TypeNameContext>(0);
}

dzParser::ExpressionContext* dzParser::FieldContext::expression() {
  return getRuleContext<dzParser::ExpressionContext>(0);
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
  enterRule(_localctx, 12, dzParser::RuleField);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(90);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx)) {
    case 1: {
      setState(89);
      typeName();
      break;
    }

    default:
      break;
    }
    setState(92);
    match(dzParser::ID);
    setState(95);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx)) {
    case 1: {
      setState(93);
      match(dzParser::T__4);
      setState(94);
      expression(0);
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

dzParser::BlockContext* dzParser::FunctionContext::block() {
  return getRuleContext<dzParser::BlockContext>(0);
}

tree::TerminalNode* dzParser::FunctionContext::ID() {
  return getToken(dzParser::ID, 0);
}

std::vector<dzParser::ArgumentContext *> dzParser::FunctionContext::argument() {
  return getRuleContexts<dzParser::ArgumentContext>();
}

dzParser::ArgumentContext* dzParser::FunctionContext::argument(size_t i) {
  return getRuleContext<dzParser::ArgumentContext>(i);
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
  enterRule(_localctx, 14, dzParser::RuleFunction);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(145);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case dzParser::T__7: {
        enterOuterAlt(_localctx, 1);
        setState(97);
        antlrcpp::downCast<FunctionContext *>(_localctx)->attribute = match(dzParser::T__7);
        setState(98);
        typeName();
        setState(99);
        antlrcpp::downCast<FunctionContext *>(_localctx)->name = match(dzParser::ID);
        setState(100);
        match(dzParser::T__8);
        setState(109);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << dzParser::T__8)
          | (1ULL << dzParser::T__11)
          | (1ULL << dzParser::T__21)
          | (1ULL << dzParser::ID))) != 0)) {
          setState(101);
          argument();
          setState(106);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == dzParser::T__5) {
            setState(102);
            match(dzParser::T__5);
            setState(103);
            argument();
            setState(108);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
        }
        setState(111);
        match(dzParser::T__9);
        setState(112);
        block();
        break;
      }

      case dzParser::T__10: {
        enterOuterAlt(_localctx, 2);
        setState(114);
        antlrcpp::downCast<FunctionContext *>(_localctx)->attribute = match(dzParser::T__10);
        setState(115);
        typeName();
        setState(116);
        antlrcpp::downCast<FunctionContext *>(_localctx)->name = match(dzParser::ID);
        setState(117);
        match(dzParser::T__8);
        setState(126);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << dzParser::T__8)
          | (1ULL << dzParser::T__11)
          | (1ULL << dzParser::T__21)
          | (1ULL << dzParser::ID))) != 0)) {
          setState(118);
          argument();
          setState(123);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == dzParser::T__5) {
            setState(119);
            match(dzParser::T__5);
            setState(120);
            argument();
            setState(125);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
        }
        setState(128);
        match(dzParser::T__9);
        break;
      }

      case dzParser::T__11: {
        enterOuterAlt(_localctx, 3);
        setState(130);
        match(dzParser::T__11);
        setState(131);
        antlrcpp::downCast<FunctionContext *>(_localctx)->name = match(dzParser::ID);
        setState(132);
        match(dzParser::T__8);
        setState(141);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << dzParser::T__8)
          | (1ULL << dzParser::T__11)
          | (1ULL << dzParser::T__21)
          | (1ULL << dzParser::ID))) != 0)) {
          setState(133);
          argument();
          setState(138);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == dzParser::T__5) {
            setState(134);
            match(dzParser::T__5);
            setState(135);
            argument();
            setState(140);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
        }
        setState(143);
        match(dzParser::T__9);
        setState(144);
        block();
        break;
      }

    default:
      throw NoViableAltException(this);
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

//----------------- ByteLiteralContext ------------------------------------------------------------------

tree::TerminalNode* dzParser::ByteLiteralContext::INT() {
  return getToken(dzParser::INT, 0);
}

dzParser::ByteLiteralContext::ByteLiteralContext(LiteralContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::ByteLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitByteLiteral(this);
  else
    return visitor->visitChildren(this);
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
//----------------- NothingContext ------------------------------------------------------------------

dzParser::NothingContext::NothingContext(LiteralContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::NothingContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitNothing(this);
  else
    return visitor->visitChildren(this);
}
//----------------- CharLiteralContext ------------------------------------------------------------------

tree::TerminalNode* dzParser::CharLiteralContext::CHARACTER() {
  return getToken(dzParser::CHARACTER, 0);
}

dzParser::CharLiteralContext::CharLiteralContext(LiteralContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::CharLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitCharLiteral(this);
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
  enterRule(_localctx, 16, dzParser::RuleLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(158);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<dzParser::Int32LiteralContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(147);
      match(dzParser::INT);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<dzParser::Int64LiteralContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(148);
      match(dzParser::INT);
      setState(149);
      match(dzParser::T__12);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<dzParser::BoolLiteralContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(150);
      match(dzParser::BOOL);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<dzParser::StringLiteralContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(151);
      match(dzParser::STRING);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<dzParser::Uint32LiteralContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(152);
      match(dzParser::INT);
      setState(153);
      match(dzParser::T__13);
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<dzParser::CharLiteralContext>(_localctx);
      enterOuterAlt(_localctx, 6);
      setState(154);
      match(dzParser::CHARACTER);
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<dzParser::ByteLiteralContext>(_localctx);
      enterOuterAlt(_localctx, 7);
      setState(155);
      match(dzParser::INT);
      setState(156);
      match(dzParser::T__14);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<dzParser::NothingContext>(_localctx);
      enterOuterAlt(_localctx, 8);
      setState(157);
      match(dzParser::T__15);
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

//----------------- WithContext ------------------------------------------------------------------

dzParser::WithContext::WithContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<dzParser::AssignmentContext *> dzParser::WithContext::assignment() {
  return getRuleContexts<dzParser::AssignmentContext>();
}

dzParser::AssignmentContext* dzParser::WithContext::assignment(size_t i) {
  return getRuleContext<dzParser::AssignmentContext>(i);
}


size_t dzParser::WithContext::getRuleIndex() const {
  return dzParser::RuleWith;
}


antlrcpp::Any dzParser::WithContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitWith(this);
  else
    return visitor->visitChildren(this);
}

dzParser::WithContext* dzParser::with() {
  WithContext *_localctx = _tracker.createInstance<WithContext>(_ctx, getState());
  enterRule(_localctx, 18, dzParser::RuleWith);
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
    setState(160);
    match(dzParser::T__16);
    setState(161);
    match(dzParser::T__1);
    setState(162);
    assignment();
    setState(167);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == dzParser::T__5) {
      setState(163);
      match(dzParser::T__5);
      setState(164);
      assignment();
      setState(169);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(170);
    match(dzParser::T__2);
   
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
//----------------- ArrayContext ------------------------------------------------------------------

std::vector<dzParser::ExpressionContext *> dzParser::ArrayContext::expression() {
  return getRuleContexts<dzParser::ExpressionContext>();
}

dzParser::ExpressionContext* dzParser::ArrayContext::expression(size_t i) {
  return getRuleContext<dzParser::ExpressionContext>(i);
}

dzParser::ArrayContext::ArrayContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::ArrayContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitArray(this);
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

dzParser::WithContext* dzParser::MemberContext::with() {
  return getRuleContext<dzParser::WithContext>(0);
}

dzParser::MemberContext::MemberContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::MemberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitMember(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LocalContext ------------------------------------------------------------------

tree::TerminalNode* dzParser::LocalContext::ID() {
  return getToken(dzParser::ID, 0);
}

dzParser::ExpressionContext* dzParser::LocalContext::expression() {
  return getRuleContext<dzParser::ExpressionContext>(0);
}

dzParser::LocalContext::LocalContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::LocalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitLocal(this);
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
//----------------- GroupContext ------------------------------------------------------------------

dzParser::ExpressionContext* dzParser::GroupContext::expression() {
  return getRuleContext<dzParser::ExpressionContext>(0);
}

dzParser::GroupContext::GroupContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::GroupContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitGroup(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExpansionContext ------------------------------------------------------------------

dzParser::ExpressionContext* dzParser::ExpansionContext::expression() {
  return getRuleContext<dzParser::ExpressionContext>(0);
}

dzParser::ExpansionContext::ExpansionContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::ExpansionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitExpansion(this);
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
  size_t startState = 20;
  enterRecursionRule(_localctx, 20, dzParser::RuleExpression, precedence);

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
    setState(240);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<CallContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(173);
      match(dzParser::ID);
      setState(174);
      match(dzParser::T__8);
      setState(183);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << dzParser::T__8)
        | (1ULL << dzParser::T__11)
        | (1ULL << dzParser::T__15)
        | (1ULL << dzParser::T__18)
        | (1ULL << dzParser::T__19)
        | (1ULL << dzParser::T__21)
        | (1ULL << dzParser::T__22)
        | (1ULL << dzParser::INT)
        | (1ULL << dzParser::BOOL)
        | (1ULL << dzParser::STRING)
        | (1ULL << dzParser::CHARACTER)
        | (1ULL << dzParser::ID))) != 0)) {
        setState(175);
        expression(0);
        setState(180);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == dzParser::T__5) {
          setState(176);
          match(dzParser::T__5);
          setState(177);
          expression(0);
          setState(182);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(185);
      match(dzParser::T__9);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<ConstantContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(186);
      literal();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MemberContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(187);
      match(dzParser::ID);
      setState(192);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(188);
          match(dzParser::T__17);
          setState(189);
          match(dzParser::ID); 
        }
        setState(194);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx);
      }
      setState(196);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx)) {
      case 1: {
        setState(195);
        with();
        break;
      }

      default:
        break;
      }
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<InstantiationContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(198);
      typeName();
      setState(199);
      match(dzParser::T__1);
      setState(201);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << dzParser::T__11)
        | (1ULL << dzParser::T__21)
        | (1ULL << dzParser::ID))) != 0)) {
        setState(200);
        assignment();
      }
      setState(207);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == dzParser::T__5) {
        setState(203);
        match(dzParser::T__5);
        setState(204);
        assignment();
        setState(209);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(210);
      match(dzParser::T__2);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<ConditionalContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(212);
      match(dzParser::T__18);
      setState(213);
      match(dzParser::T__8);
      setState(214);
      expression(0);
      setState(215);
      match(dzParser::T__9);
      setState(216);
      block();
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<ArrayContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(218);
      match(dzParser::T__19);
      setState(220);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << dzParser::T__8)
        | (1ULL << dzParser::T__11)
        | (1ULL << dzParser::T__15)
        | (1ULL << dzParser::T__18)
        | (1ULL << dzParser::T__19)
        | (1ULL << dzParser::T__21)
        | (1ULL << dzParser::T__22)
        | (1ULL << dzParser::INT)
        | (1ULL << dzParser::BOOL)
        | (1ULL << dzParser::STRING)
        | (1ULL << dzParser::CHARACTER)
        | (1ULL << dzParser::ID))) != 0)) {
        setState(219);
        expression(0);
      }
      setState(226);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == dzParser::T__5) {
        setState(222);
        match(dzParser::T__5);
        setState(223);
        expression(0);
        setState(228);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(229);
      match(dzParser::T__20);
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<GroupContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(230);
      match(dzParser::T__8);
      setState(231);
      expression(0);
      setState(232);
      match(dzParser::T__9);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<ExpansionContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(234);
      match(dzParser::T__21);
      setState(235);
      expression(2);
      break;
    }

    case 9: {
      _localctx = _tracker.createInstance<LocalContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(236);
      match(dzParser::T__22);
      setState(237);
      match(dzParser::ID);
      setState(238);
      match(dzParser::T__23);
      setState(239);
      expression(1);
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(247);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 26, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        auto newContext = _tracker.createInstance<BinaryContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
        _localctx = newContext;
        newContext->left = previousContext;
        pushNewRecursionContext(newContext, startState, RuleExpression);
        setState(242);

        if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
        setState(243);
        antlrcpp::downCast<BinaryContext *>(_localctx)->op = match(dzParser::OP);
        setState(244);
        antlrcpp::downCast<BinaryContext *>(_localctx)->right = expression(9); 
      }
      setState(249);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 26, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- ContinuationContext ------------------------------------------------------------------

dzParser::ContinuationContext::ContinuationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<dzParser::ExpressionContext *> dzParser::ContinuationContext::expression() {
  return getRuleContexts<dzParser::ExpressionContext>();
}

dzParser::ExpressionContext* dzParser::ContinuationContext::expression(size_t i) {
  return getRuleContext<dzParser::ExpressionContext>(i);
}


size_t dzParser::ContinuationContext::getRuleIndex() const {
  return dzParser::RuleContinuation;
}


antlrcpp::Any dzParser::ContinuationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitContinuation(this);
  else
    return visitor->visitChildren(this);
}

dzParser::ContinuationContext* dzParser::continuation() {
  ContinuationContext *_localctx = _tracker.createInstance<ContinuationContext>(_ctx, getState());
  enterRule(_localctx, 22, dzParser::RuleContinuation);
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
    setState(250);
    match(dzParser::T__8);
    setState(259);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << dzParser::T__8)
      | (1ULL << dzParser::T__11)
      | (1ULL << dzParser::T__15)
      | (1ULL << dzParser::T__18)
      | (1ULL << dzParser::T__19)
      | (1ULL << dzParser::T__21)
      | (1ULL << dzParser::T__22)
      | (1ULL << dzParser::INT)
      | (1ULL << dzParser::BOOL)
      | (1ULL << dzParser::STRING)
      | (1ULL << dzParser::CHARACTER)
      | (1ULL << dzParser::ID))) != 0)) {
      setState(251);
      expression(0);
      setState(256);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == dzParser::T__5) {
        setState(252);
        match(dzParser::T__5);
        setState(253);
        expression(0);
        setState(258);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(261);
    match(dzParser::T__9);
   
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

dzParser::ExpressionContext* dzParser::RetContext::expression() {
  return getRuleContext<dzParser::ExpressionContext>(0);
}

dzParser::ContinuationContext* dzParser::RetContext::continuation() {
  return getRuleContext<dzParser::ContinuationContext>(0);
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
  enterRule(_localctx, 24, dzParser::RuleRet);
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
    setState(263);
    match(dzParser::T__24);
    setState(265);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << dzParser::T__8)
      | (1ULL << dzParser::T__11)
      | (1ULL << dzParser::T__15)
      | (1ULL << dzParser::T__18)
      | (1ULL << dzParser::T__19)
      | (1ULL << dzParser::T__21)
      | (1ULL << dzParser::T__22)
      | (1ULL << dzParser::INT)
      | (1ULL << dzParser::BOOL)
      | (1ULL << dzParser::STRING)
      | (1ULL << dzParser::CHARACTER)
      | (1ULL << dzParser::ID))) != 0)) {
      setState(264);
      antlrcpp::downCast<RetContext *>(_localctx)->value = expression(0);
    }
    setState(269);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == dzParser::T__25) {
      setState(267);
      match(dzParser::T__25);
      setState(268);
      antlrcpp::downCast<RetContext *>(_localctx)->chained = continuation();
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
  enterRule(_localctx, 26, dzParser::RuleBlock);
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
    setState(271);
    match(dzParser::T__1);
    setState(275);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << dzParser::T__8)
      | (1ULL << dzParser::T__11)
      | (1ULL << dzParser::T__15)
      | (1ULL << dzParser::T__18)
      | (1ULL << dzParser::T__19)
      | (1ULL << dzParser::T__21)
      | (1ULL << dzParser::T__22)
      | (1ULL << dzParser::INT)
      | (1ULL << dzParser::BOOL)
      | (1ULL << dzParser::STRING)
      | (1ULL << dzParser::CHARACTER)
      | (1ULL << dzParser::ID))) != 0)) {
      setState(272);
      expression(0);
      setState(277);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(278);
    ret();
    setState(279);
    match(dzParser::T__2);
   
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


size_t dzParser::ArgumentContext::getRuleIndex() const {
  return dzParser::RuleArgument;
}

void dzParser::ArgumentContext::copyFrom(ArgumentContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- StandardArgumentContext ------------------------------------------------------------------

dzParser::TypeNameContext* dzParser::StandardArgumentContext::typeName() {
  return getRuleContext<dzParser::TypeNameContext>(0);
}

tree::TerminalNode* dzParser::StandardArgumentContext::ID() {
  return getToken(dzParser::ID, 0);
}

dzParser::StandardArgumentContext::StandardArgumentContext(ArgumentContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::StandardArgumentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitStandardArgument(this);
  else
    return visitor->visitChildren(this);
}
//----------------- TupleArgumentContext ------------------------------------------------------------------

std::vector<dzParser::ArgumentContext *> dzParser::TupleArgumentContext::argument() {
  return getRuleContexts<dzParser::ArgumentContext>();
}

dzParser::ArgumentContext* dzParser::TupleArgumentContext::argument(size_t i) {
  return getRuleContext<dzParser::ArgumentContext>(i);
}

dzParser::TupleArgumentContext::TupleArgumentContext(ArgumentContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::TupleArgumentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitTupleArgument(this);
  else
    return visitor->visitChildren(this);
}
dzParser::ArgumentContext* dzParser::argument() {
  ArgumentContext *_localctx = _tracker.createInstance<ArgumentContext>(_ctx, getState());
  enterRule(_localctx, 28, dzParser::RuleArgument);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(295);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case dzParser::T__11:
      case dzParser::T__21:
      case dzParser::ID: {
        _localctx = _tracker.createInstance<dzParser::StandardArgumentContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(281);
        typeName();
        setState(282);
        match(dzParser::ID);
        break;
      }

      case dzParser::T__8: {
        _localctx = _tracker.createInstance<dzParser::TupleArgumentContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(284);
        match(dzParser::T__8);
        setState(285);
        argument();
        setState(290);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == dzParser::T__5) {
          setState(286);
          match(dzParser::T__5);
          setState(287);
          argument();
          setState(292);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(293);
        match(dzParser::T__9);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
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


size_t dzParser::TypeNameContext::getRuleIndex() const {
  return dzParser::RuleTypeName;
}

void dzParser::TypeNameContext::copyFrom(TypeNameContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- FunctionTypeContext ------------------------------------------------------------------

std::vector<dzParser::TypeNameContext *> dzParser::FunctionTypeContext::typeName() {
  return getRuleContexts<dzParser::TypeNameContext>();
}

dzParser::TypeNameContext* dzParser::FunctionTypeContext::typeName(size_t i) {
  return getRuleContext<dzParser::TypeNameContext>(i);
}

dzParser::FunctionTypeContext::FunctionTypeContext(TypeNameContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::FunctionTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitFunctionType(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RegularTypeContext ------------------------------------------------------------------

tree::TerminalNode* dzParser::RegularTypeContext::ID() {
  return getToken(dzParser::ID, 0);
}

dzParser::RegularTypeContext::RegularTypeContext(TypeNameContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::RegularTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitRegularType(this);
  else
    return visitor->visitChildren(this);
}
dzParser::TypeNameContext* dzParser::typeName() {
  TypeNameContext *_localctx = _tracker.createInstance<TypeNameContext>(_ctx, getState());
  enterRule(_localctx, 30, dzParser::RuleTypeName);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(310);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case dzParser::T__21:
      case dzParser::ID: {
        _localctx = _tracker.createInstance<dzParser::RegularTypeContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(297);
        _la = _input->LA(1);
        if (!(_la == dzParser::T__21

        || _la == dzParser::ID)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        break;
      }

      case dzParser::T__11: {
        _localctx = _tracker.createInstance<dzParser::FunctionTypeContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(298);
        match(dzParser::T__11);
        setState(299);
        match(dzParser::T__8);

        setState(300);
        typeName();
        setState(305);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == dzParser::T__5) {
          setState(301);
          match(dzParser::T__5);
          setState(302);
          typeName();
          setState(307);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(308);
        match(dzParser::T__9);
        break;
      }

    default:
      throw NoViableAltException(this);
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
    case 10: return expressionSempred(antlrcpp::downCast<ExpressionContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool dzParser::expressionSempred(ExpressionContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 8);

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
  "program", "instruction", "ns", "structure", "global", "assignment", "field", 
  "function", "literal", "with", "expression", "continuation", "ret", "block", 
  "argument", "typeName"
};

std::vector<std::string> dzParser::_literalNames = {
  "", "'namespace'", "'{'", "'}'", "'struct'", "':'", "','", "'global'", 
  "'export'", "'('", "')'", "'import'", "'function'", "'L'", "'u'", "'u8'", 
  "'nothing'", "'with'", "'.'", "'if'", "'['", "']'", "'...'", "'let'", 
  "'='", "'return'", "'->'"
};

std::vector<std::string> dzParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "INT", "BOOL", "OP", "COMMENT", "STRING", 
  "CHARACTER", "WS", "ID"
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
       0x3, 0x24, 0x13b, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
       0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
       0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 
       0x4, 0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 
       0xe, 0x9, 0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 
       0x9, 0x11, 0x3, 0x2, 0x7, 0x2, 0x24, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 
       0x27, 0xb, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
       0x5, 0x3, 0x2e, 0xa, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
       0x7, 0x4, 0x34, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x37, 0xb, 0x4, 0x3, 
       0x4, 0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 
       0x3, 0x5, 0x7, 0x5, 0x41, 0xa, 0x5, 0xc, 0x5, 0xe, 0x5, 0x44, 0xb, 
       0x5, 0x5, 0x5, 0x46, 0xa, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 
       0x5, 0x7, 0x5, 0x4c, 0xa, 0x5, 0xc, 0x5, 0xe, 0x5, 0x4f, 0xb, 0x5, 
       0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
       0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x8, 0x5, 0x8, 
       0x5d, 0xa, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x5, 0x8, 0x62, 0xa, 
       0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 
       0x3, 0x9, 0x7, 0x9, 0x6b, 0xa, 0x9, 0xc, 0x9, 0xe, 0x9, 0x6e, 0xb, 
       0x9, 0x5, 0x9, 0x70, 0xa, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 
       0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 
       0x7, 0x9, 0x7c, 0xa, 0x9, 0xc, 0x9, 0xe, 0x9, 0x7f, 0xb, 0x9, 0x5, 
       0x9, 0x81, 0xa, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 
       0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x7, 0x9, 0x8b, 0xa, 0x9, 0xc, 
       0x9, 0xe, 0x9, 0x8e, 0xb, 0x9, 0x5, 0x9, 0x90, 0xa, 0x9, 0x3, 0x9, 
       0x3, 0x9, 0x5, 0x9, 0x94, 0xa, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 
       0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 
       0xa, 0x3, 0xa, 0x5, 0xa, 0xa1, 0xa, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 
       0xb, 0x3, 0xb, 0x3, 0xb, 0x7, 0xb, 0xa8, 0xa, 0xb, 0xc, 0xb, 0xe, 
       0xb, 0xab, 0xb, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 
       0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x7, 0xc, 0xb5, 0xa, 0xc, 0xc, 
       0xc, 0xe, 0xc, 0xb8, 0xb, 0xc, 0x5, 0xc, 0xba, 0xa, 0xc, 0x3, 0xc, 
       0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x7, 0xc, 0xc1, 0xa, 0xc, 
       0xc, 0xc, 0xe, 0xc, 0xc4, 0xb, 0xc, 0x3, 0xc, 0x5, 0xc, 0xc7, 0xa, 
       0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x5, 0xc, 0xcc, 0xa, 0xc, 0x3, 
       0xc, 0x3, 0xc, 0x7, 0xc, 0xd0, 0xa, 0xc, 0xc, 0xc, 0xe, 0xc, 0xd3, 
       0xb, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 
       0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x5, 0xc, 0xdf, 0xa, 
       0xc, 0x3, 0xc, 0x3, 0xc, 0x7, 0xc, 0xe3, 0xa, 0xc, 0xc, 0xc, 0xe, 
       0xc, 0xe6, 0xb, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 
       0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 
       0x5, 0xc, 0xf3, 0xa, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x7, 0xc, 
       0xf8, 0xa, 0xc, 0xc, 0xc, 0xe, 0xc, 0xfb, 0xb, 0xc, 0x3, 0xd, 0x3, 
       0xd, 0x3, 0xd, 0x3, 0xd, 0x7, 0xd, 0x101, 0xa, 0xd, 0xc, 0xd, 0xe, 
       0xd, 0x104, 0xb, 0xd, 0x5, 0xd, 0x106, 0xa, 0xd, 0x3, 0xd, 0x3, 0xd, 
       0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 0x10c, 0xa, 0xe, 0x3, 0xe, 0x3, 0xe, 
       0x5, 0xe, 0x110, 0xa, 0xe, 0x3, 0xf, 0x3, 0xf, 0x7, 0xf, 0x114, 0xa, 
       0xf, 0xc, 0xf, 0xe, 0xf, 0x117, 0xb, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 
       0xf, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 
       0x10, 0x3, 0x10, 0x7, 0x10, 0x123, 0xa, 0x10, 0xc, 0x10, 0xe, 0x10, 
       0x126, 0xb, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x12a, 0xa, 0x10, 
       0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 
       0x7, 0x11, 0x132, 0xa, 0x11, 0xc, 0x11, 0xe, 0x11, 0x135, 0xb, 0x11, 
       0x3, 0x11, 0x3, 0x11, 0x5, 0x11, 0x139, 0xa, 0x11, 0x3, 0x11, 0x2, 
       0x3, 0x16, 0x12, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 
       0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x2, 0x3, 0x4, 0x2, 0x18, 0x18, 
       0x24, 0x24, 0x2, 0x15f, 0x2, 0x25, 0x3, 0x2, 0x2, 0x2, 0x4, 0x2d, 
       0x3, 0x2, 0x2, 0x2, 0x6, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x8, 0x3a, 0x3, 
       0x2, 0x2, 0x2, 0xa, 0x52, 0x3, 0x2, 0x2, 0x2, 0xc, 0x57, 0x3, 0x2, 
       0x2, 0x2, 0xe, 0x5c, 0x3, 0x2, 0x2, 0x2, 0x10, 0x93, 0x3, 0x2, 0x2, 
       0x2, 0x12, 0xa0, 0x3, 0x2, 0x2, 0x2, 0x14, 0xa2, 0x3, 0x2, 0x2, 0x2, 
       0x16, 0xf2, 0x3, 0x2, 0x2, 0x2, 0x18, 0xfc, 0x3, 0x2, 0x2, 0x2, 0x1a, 
       0x109, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x111, 0x3, 0x2, 0x2, 0x2, 0x1e, 
       0x129, 0x3, 0x2, 0x2, 0x2, 0x20, 0x138, 0x3, 0x2, 0x2, 0x2, 0x22, 
       0x24, 0x5, 0x4, 0x3, 0x2, 0x23, 0x22, 0x3, 0x2, 0x2, 0x2, 0x24, 0x27, 
       0x3, 0x2, 0x2, 0x2, 0x25, 0x23, 0x3, 0x2, 0x2, 0x2, 0x25, 0x26, 0x3, 
       0x2, 0x2, 0x2, 0x26, 0x3, 0x3, 0x2, 0x2, 0x2, 0x27, 0x25, 0x3, 0x2, 
       0x2, 0x2, 0x28, 0x2e, 0x7, 0x20, 0x2, 0x2, 0x29, 0x2e, 0x5, 0x10, 
       0x9, 0x2, 0x2a, 0x2e, 0x5, 0x8, 0x5, 0x2, 0x2b, 0x2e, 0x5, 0xa, 0x6, 
       0x2, 0x2c, 0x2e, 0x5, 0x6, 0x4, 0x2, 0x2d, 0x28, 0x3, 0x2, 0x2, 0x2, 
       0x2d, 0x29, 0x3, 0x2, 0x2, 0x2, 0x2d, 0x2a, 0x3, 0x2, 0x2, 0x2, 0x2d, 
       0x2b, 0x3, 0x2, 0x2, 0x2, 0x2d, 0x2c, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x5, 
       0x3, 0x2, 0x2, 0x2, 0x2f, 0x30, 0x7, 0x3, 0x2, 0x2, 0x30, 0x31, 0x7, 
       0x24, 0x2, 0x2, 0x31, 0x35, 0x7, 0x4, 0x2, 0x2, 0x32, 0x34, 0x5, 
       0x4, 0x3, 0x2, 0x33, 0x32, 0x3, 0x2, 0x2, 0x2, 0x34, 0x37, 0x3, 0x2, 
       0x2, 0x2, 0x35, 0x33, 0x3, 0x2, 0x2, 0x2, 0x35, 0x36, 0x3, 0x2, 0x2, 
       0x2, 0x36, 0x38, 0x3, 0x2, 0x2, 0x2, 0x37, 0x35, 0x3, 0x2, 0x2, 0x2, 
       0x38, 0x39, 0x7, 0x5, 0x2, 0x2, 0x39, 0x7, 0x3, 0x2, 0x2, 0x2, 0x3a, 
       0x3b, 0x7, 0x6, 0x2, 0x2, 0x3b, 0x45, 0x7, 0x24, 0x2, 0x2, 0x3c, 
       0x3d, 0x7, 0x7, 0x2, 0x2, 0x3d, 0x42, 0x5, 0x20, 0x11, 0x2, 0x3e, 
       0x3f, 0x7, 0x8, 0x2, 0x2, 0x3f, 0x41, 0x5, 0x20, 0x11, 0x2, 0x40, 
       0x3e, 0x3, 0x2, 0x2, 0x2, 0x41, 0x44, 0x3, 0x2, 0x2, 0x2, 0x42, 0x40, 
       0x3, 0x2, 0x2, 0x2, 0x42, 0x43, 0x3, 0x2, 0x2, 0x2, 0x43, 0x46, 0x3, 
       0x2, 0x2, 0x2, 0x44, 0x42, 0x3, 0x2, 0x2, 0x2, 0x45, 0x3c, 0x3, 0x2, 
       0x2, 0x2, 0x45, 0x46, 0x3, 0x2, 0x2, 0x2, 0x46, 0x47, 0x3, 0x2, 0x2, 
       0x2, 0x47, 0x48, 0x7, 0x4, 0x2, 0x2, 0x48, 0x4d, 0x5, 0xe, 0x8, 0x2, 
       0x49, 0x4a, 0x7, 0x8, 0x2, 0x2, 0x4a, 0x4c, 0x5, 0xe, 0x8, 0x2, 0x4b, 
       0x49, 0x3, 0x2, 0x2, 0x2, 0x4c, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x4b, 
       0x3, 0x2, 0x2, 0x2, 0x4d, 0x4e, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x50, 0x3, 
       0x2, 0x2, 0x2, 0x4f, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x50, 0x51, 0x7, 0x5, 
       0x2, 0x2, 0x51, 0x9, 0x3, 0x2, 0x2, 0x2, 0x52, 0x53, 0x7, 0x9, 0x2, 
       0x2, 0x53, 0x54, 0x7, 0x24, 0x2, 0x2, 0x54, 0x55, 0x7, 0x7, 0x2, 
       0x2, 0x55, 0x56, 0x5, 0x12, 0xa, 0x2, 0x56, 0xb, 0x3, 0x2, 0x2, 0x2, 
       0x57, 0x58, 0x5, 0xe, 0x8, 0x2, 0x58, 0x59, 0x7, 0x7, 0x2, 0x2, 0x59, 
       0x5a, 0x5, 0x16, 0xc, 0x2, 0x5a, 0xd, 0x3, 0x2, 0x2, 0x2, 0x5b, 0x5d, 
       0x5, 0x20, 0x11, 0x2, 0x5c, 0x5b, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x5d, 
       0x3, 0x2, 0x2, 0x2, 0x5d, 0x5e, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x61, 0x7, 
       0x24, 0x2, 0x2, 0x5f, 0x60, 0x7, 0x7, 0x2, 0x2, 0x60, 0x62, 0x5, 
       0x16, 0xc, 0x2, 0x61, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x61, 0x62, 0x3, 
       0x2, 0x2, 0x2, 0x62, 0xf, 0x3, 0x2, 0x2, 0x2, 0x63, 0x64, 0x7, 0xa, 
       0x2, 0x2, 0x64, 0x65, 0x5, 0x20, 0x11, 0x2, 0x65, 0x66, 0x7, 0x24, 
       0x2, 0x2, 0x66, 0x6f, 0x7, 0xb, 0x2, 0x2, 0x67, 0x6c, 0x5, 0x1e, 
       0x10, 0x2, 0x68, 0x69, 0x7, 0x8, 0x2, 0x2, 0x69, 0x6b, 0x5, 0x1e, 
       0x10, 0x2, 0x6a, 0x68, 0x3, 0x2, 0x2, 0x2, 0x6b, 0x6e, 0x3, 0x2, 
       0x2, 0x2, 0x6c, 0x6a, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x6d, 0x3, 0x2, 0x2, 
       0x2, 0x6d, 0x70, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x6c, 0x3, 0x2, 0x2, 0x2, 
       0x6f, 0x67, 0x3, 0x2, 0x2, 0x2, 0x6f, 0x70, 0x3, 0x2, 0x2, 0x2, 0x70, 
       0x71, 0x3, 0x2, 0x2, 0x2, 0x71, 0x72, 0x7, 0xc, 0x2, 0x2, 0x72, 0x73, 
       0x5, 0x1c, 0xf, 0x2, 0x73, 0x94, 0x3, 0x2, 0x2, 0x2, 0x74, 0x75, 
       0x7, 0xd, 0x2, 0x2, 0x75, 0x76, 0x5, 0x20, 0x11, 0x2, 0x76, 0x77, 
       0x7, 0x24, 0x2, 0x2, 0x77, 0x80, 0x7, 0xb, 0x2, 0x2, 0x78, 0x7d, 
       0x5, 0x1e, 0x10, 0x2, 0x79, 0x7a, 0x7, 0x8, 0x2, 0x2, 0x7a, 0x7c, 
       0x5, 0x1e, 0x10, 0x2, 0x7b, 0x79, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x7f, 
       0x3, 0x2, 0x2, 0x2, 0x7d, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x7d, 0x7e, 0x3, 
       0x2, 0x2, 0x2, 0x7e, 0x81, 0x3, 0x2, 0x2, 0x2, 0x7f, 0x7d, 0x3, 0x2, 
       0x2, 0x2, 0x80, 0x78, 0x3, 0x2, 0x2, 0x2, 0x80, 0x81, 0x3, 0x2, 0x2, 
       0x2, 0x81, 0x82, 0x3, 0x2, 0x2, 0x2, 0x82, 0x83, 0x7, 0xc, 0x2, 0x2, 
       0x83, 0x94, 0x3, 0x2, 0x2, 0x2, 0x84, 0x85, 0x7, 0xe, 0x2, 0x2, 0x85, 
       0x86, 0x7, 0x24, 0x2, 0x2, 0x86, 0x8f, 0x7, 0xb, 0x2, 0x2, 0x87, 
       0x8c, 0x5, 0x1e, 0x10, 0x2, 0x88, 0x89, 0x7, 0x8, 0x2, 0x2, 0x89, 
       0x8b, 0x5, 0x1e, 0x10, 0x2, 0x8a, 0x88, 0x3, 0x2, 0x2, 0x2, 0x8b, 
       0x8e, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x8a, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x8d, 
       0x3, 0x2, 0x2, 0x2, 0x8d, 0x90, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x8c, 0x3, 
       0x2, 0x2, 0x2, 0x8f, 0x87, 0x3, 0x2, 0x2, 0x2, 0x8f, 0x90, 0x3, 0x2, 
       0x2, 0x2, 0x90, 0x91, 0x3, 0x2, 0x2, 0x2, 0x91, 0x92, 0x7, 0xc, 0x2, 
       0x2, 0x92, 0x94, 0x5, 0x1c, 0xf, 0x2, 0x93, 0x63, 0x3, 0x2, 0x2, 
       0x2, 0x93, 0x74, 0x3, 0x2, 0x2, 0x2, 0x93, 0x84, 0x3, 0x2, 0x2, 0x2, 
       0x94, 0x11, 0x3, 0x2, 0x2, 0x2, 0x95, 0xa1, 0x7, 0x1d, 0x2, 0x2, 
       0x96, 0x97, 0x7, 0x1d, 0x2, 0x2, 0x97, 0xa1, 0x7, 0xf, 0x2, 0x2, 
       0x98, 0xa1, 0x7, 0x1e, 0x2, 0x2, 0x99, 0xa1, 0x7, 0x21, 0x2, 0x2, 
       0x9a, 0x9b, 0x7, 0x1d, 0x2, 0x2, 0x9b, 0xa1, 0x7, 0x10, 0x2, 0x2, 
       0x9c, 0xa1, 0x7, 0x22, 0x2, 0x2, 0x9d, 0x9e, 0x7, 0x1d, 0x2, 0x2, 
       0x9e, 0xa1, 0x7, 0x11, 0x2, 0x2, 0x9f, 0xa1, 0x7, 0x12, 0x2, 0x2, 
       0xa0, 0x95, 0x3, 0x2, 0x2, 0x2, 0xa0, 0x96, 0x3, 0x2, 0x2, 0x2, 0xa0, 
       0x98, 0x3, 0x2, 0x2, 0x2, 0xa0, 0x99, 0x3, 0x2, 0x2, 0x2, 0xa0, 0x9a, 
       0x3, 0x2, 0x2, 0x2, 0xa0, 0x9c, 0x3, 0x2, 0x2, 0x2, 0xa0, 0x9d, 0x3, 
       0x2, 0x2, 0x2, 0xa0, 0x9f, 0x3, 0x2, 0x2, 0x2, 0xa1, 0x13, 0x3, 0x2, 
       0x2, 0x2, 0xa2, 0xa3, 0x7, 0x13, 0x2, 0x2, 0xa3, 0xa4, 0x7, 0x4, 
       0x2, 0x2, 0xa4, 0xa9, 0x5, 0xc, 0x7, 0x2, 0xa5, 0xa6, 0x7, 0x8, 0x2, 
       0x2, 0xa6, 0xa8, 0x5, 0xc, 0x7, 0x2, 0xa7, 0xa5, 0x3, 0x2, 0x2, 0x2, 
       0xa8, 0xab, 0x3, 0x2, 0x2, 0x2, 0xa9, 0xa7, 0x3, 0x2, 0x2, 0x2, 0xa9, 
       0xaa, 0x3, 0x2, 0x2, 0x2, 0xaa, 0xac, 0x3, 0x2, 0x2, 0x2, 0xab, 0xa9, 
       0x3, 0x2, 0x2, 0x2, 0xac, 0xad, 0x7, 0x5, 0x2, 0x2, 0xad, 0x15, 0x3, 
       0x2, 0x2, 0x2, 0xae, 0xaf, 0x8, 0xc, 0x1, 0x2, 0xaf, 0xb0, 0x7, 0x24, 
       0x2, 0x2, 0xb0, 0xb9, 0x7, 0xb, 0x2, 0x2, 0xb1, 0xb6, 0x5, 0x16, 
       0xc, 0x2, 0xb2, 0xb3, 0x7, 0x8, 0x2, 0x2, 0xb3, 0xb5, 0x5, 0x16, 
       0xc, 0x2, 0xb4, 0xb2, 0x3, 0x2, 0x2, 0x2, 0xb5, 0xb8, 0x3, 0x2, 0x2, 
       0x2, 0xb6, 0xb4, 0x3, 0x2, 0x2, 0x2, 0xb6, 0xb7, 0x3, 0x2, 0x2, 0x2, 
       0xb7, 0xba, 0x3, 0x2, 0x2, 0x2, 0xb8, 0xb6, 0x3, 0x2, 0x2, 0x2, 0xb9, 
       0xb1, 0x3, 0x2, 0x2, 0x2, 0xb9, 0xba, 0x3, 0x2, 0x2, 0x2, 0xba, 0xbb, 
       0x3, 0x2, 0x2, 0x2, 0xbb, 0xf3, 0x7, 0xc, 0x2, 0x2, 0xbc, 0xf3, 0x5, 
       0x12, 0xa, 0x2, 0xbd, 0xc2, 0x7, 0x24, 0x2, 0x2, 0xbe, 0xbf, 0x7, 
       0x14, 0x2, 0x2, 0xbf, 0xc1, 0x7, 0x24, 0x2, 0x2, 0xc0, 0xbe, 0x3, 
       0x2, 0x2, 0x2, 0xc1, 0xc4, 0x3, 0x2, 0x2, 0x2, 0xc2, 0xc0, 0x3, 0x2, 
       0x2, 0x2, 0xc2, 0xc3, 0x3, 0x2, 0x2, 0x2, 0xc3, 0xc6, 0x3, 0x2, 0x2, 
       0x2, 0xc4, 0xc2, 0x3, 0x2, 0x2, 0x2, 0xc5, 0xc7, 0x5, 0x14, 0xb, 
       0x2, 0xc6, 0xc5, 0x3, 0x2, 0x2, 0x2, 0xc6, 0xc7, 0x3, 0x2, 0x2, 0x2, 
       0xc7, 0xf3, 0x3, 0x2, 0x2, 0x2, 0xc8, 0xc9, 0x5, 0x20, 0x11, 0x2, 
       0xc9, 0xcb, 0x7, 0x4, 0x2, 0x2, 0xca, 0xcc, 0x5, 0xc, 0x7, 0x2, 0xcb, 
       0xca, 0x3, 0x2, 0x2, 0x2, 0xcb, 0xcc, 0x3, 0x2, 0x2, 0x2, 0xcc, 0xd1, 
       0x3, 0x2, 0x2, 0x2, 0xcd, 0xce, 0x7, 0x8, 0x2, 0x2, 0xce, 0xd0, 0x5, 
       0xc, 0x7, 0x2, 0xcf, 0xcd, 0x3, 0x2, 0x2, 0x2, 0xd0, 0xd3, 0x3, 0x2, 
       0x2, 0x2, 0xd1, 0xcf, 0x3, 0x2, 0x2, 0x2, 0xd1, 0xd2, 0x3, 0x2, 0x2, 
       0x2, 0xd2, 0xd4, 0x3, 0x2, 0x2, 0x2, 0xd3, 0xd1, 0x3, 0x2, 0x2, 0x2, 
       0xd4, 0xd5, 0x7, 0x5, 0x2, 0x2, 0xd5, 0xf3, 0x3, 0x2, 0x2, 0x2, 0xd6, 
       0xd7, 0x7, 0x15, 0x2, 0x2, 0xd7, 0xd8, 0x7, 0xb, 0x2, 0x2, 0xd8, 
       0xd9, 0x5, 0x16, 0xc, 0x2, 0xd9, 0xda, 0x7, 0xc, 0x2, 0x2, 0xda, 
       0xdb, 0x5, 0x1c, 0xf, 0x2, 0xdb, 0xf3, 0x3, 0x2, 0x2, 0x2, 0xdc, 
       0xde, 0x7, 0x16, 0x2, 0x2, 0xdd, 0xdf, 0x5, 0x16, 0xc, 0x2, 0xde, 
       0xdd, 0x3, 0x2, 0x2, 0x2, 0xde, 0xdf, 0x3, 0x2, 0x2, 0x2, 0xdf, 0xe4, 
       0x3, 0x2, 0x2, 0x2, 0xe0, 0xe1, 0x7, 0x8, 0x2, 0x2, 0xe1, 0xe3, 0x5, 
       0x16, 0xc, 0x2, 0xe2, 0xe0, 0x3, 0x2, 0x2, 0x2, 0xe3, 0xe6, 0x3, 
       0x2, 0x2, 0x2, 0xe4, 0xe2, 0x3, 0x2, 0x2, 0x2, 0xe4, 0xe5, 0x3, 0x2, 
       0x2, 0x2, 0xe5, 0xe7, 0x3, 0x2, 0x2, 0x2, 0xe6, 0xe4, 0x3, 0x2, 0x2, 
       0x2, 0xe7, 0xf3, 0x7, 0x17, 0x2, 0x2, 0xe8, 0xe9, 0x7, 0xb, 0x2, 
       0x2, 0xe9, 0xea, 0x5, 0x16, 0xc, 0x2, 0xea, 0xeb, 0x7, 0xc, 0x2, 
       0x2, 0xeb, 0xf3, 0x3, 0x2, 0x2, 0x2, 0xec, 0xed, 0x7, 0x18, 0x2, 
       0x2, 0xed, 0xf3, 0x5, 0x16, 0xc, 0x4, 0xee, 0xef, 0x7, 0x19, 0x2, 
       0x2, 0xef, 0xf0, 0x7, 0x24, 0x2, 0x2, 0xf0, 0xf1, 0x7, 0x1a, 0x2, 
       0x2, 0xf1, 0xf3, 0x5, 0x16, 0xc, 0x3, 0xf2, 0xae, 0x3, 0x2, 0x2, 
       0x2, 0xf2, 0xbc, 0x3, 0x2, 0x2, 0x2, 0xf2, 0xbd, 0x3, 0x2, 0x2, 0x2, 
       0xf2, 0xc8, 0x3, 0x2, 0x2, 0x2, 0xf2, 0xd6, 0x3, 0x2, 0x2, 0x2, 0xf2, 
       0xdc, 0x3, 0x2, 0x2, 0x2, 0xf2, 0xe8, 0x3, 0x2, 0x2, 0x2, 0xf2, 0xec, 
       0x3, 0x2, 0x2, 0x2, 0xf2, 0xee, 0x3, 0x2, 0x2, 0x2, 0xf3, 0xf9, 0x3, 
       0x2, 0x2, 0x2, 0xf4, 0xf5, 0xc, 0xa, 0x2, 0x2, 0xf5, 0xf6, 0x7, 0x1f, 
       0x2, 0x2, 0xf6, 0xf8, 0x5, 0x16, 0xc, 0xb, 0xf7, 0xf4, 0x3, 0x2, 
       0x2, 0x2, 0xf8, 0xfb, 0x3, 0x2, 0x2, 0x2, 0xf9, 0xf7, 0x3, 0x2, 0x2, 
       0x2, 0xf9, 0xfa, 0x3, 0x2, 0x2, 0x2, 0xfa, 0x17, 0x3, 0x2, 0x2, 0x2, 
       0xfb, 0xf9, 0x3, 0x2, 0x2, 0x2, 0xfc, 0x105, 0x7, 0xb, 0x2, 0x2, 
       0xfd, 0x102, 0x5, 0x16, 0xc, 0x2, 0xfe, 0xff, 0x7, 0x8, 0x2, 0x2, 
       0xff, 0x101, 0x5, 0x16, 0xc, 0x2, 0x100, 0xfe, 0x3, 0x2, 0x2, 0x2, 
       0x101, 0x104, 0x3, 0x2, 0x2, 0x2, 0x102, 0x100, 0x3, 0x2, 0x2, 0x2, 
       0x102, 0x103, 0x3, 0x2, 0x2, 0x2, 0x103, 0x106, 0x3, 0x2, 0x2, 0x2, 
       0x104, 0x102, 0x3, 0x2, 0x2, 0x2, 0x105, 0xfd, 0x3, 0x2, 0x2, 0x2, 
       0x105, 0x106, 0x3, 0x2, 0x2, 0x2, 0x106, 0x107, 0x3, 0x2, 0x2, 0x2, 
       0x107, 0x108, 0x7, 0xc, 0x2, 0x2, 0x108, 0x19, 0x3, 0x2, 0x2, 0x2, 
       0x109, 0x10b, 0x7, 0x1b, 0x2, 0x2, 0x10a, 0x10c, 0x5, 0x16, 0xc, 
       0x2, 0x10b, 0x10a, 0x3, 0x2, 0x2, 0x2, 0x10b, 0x10c, 0x3, 0x2, 0x2, 
       0x2, 0x10c, 0x10f, 0x3, 0x2, 0x2, 0x2, 0x10d, 0x10e, 0x7, 0x1c, 0x2, 
       0x2, 0x10e, 0x110, 0x5, 0x18, 0xd, 0x2, 0x10f, 0x10d, 0x3, 0x2, 0x2, 
       0x2, 0x10f, 0x110, 0x3, 0x2, 0x2, 0x2, 0x110, 0x1b, 0x3, 0x2, 0x2, 
       0x2, 0x111, 0x115, 0x7, 0x4, 0x2, 0x2, 0x112, 0x114, 0x5, 0x16, 0xc, 
       0x2, 0x113, 0x112, 0x3, 0x2, 0x2, 0x2, 0x114, 0x117, 0x3, 0x2, 0x2, 
       0x2, 0x115, 0x113, 0x3, 0x2, 0x2, 0x2, 0x115, 0x116, 0x3, 0x2, 0x2, 
       0x2, 0x116, 0x118, 0x3, 0x2, 0x2, 0x2, 0x117, 0x115, 0x3, 0x2, 0x2, 
       0x2, 0x118, 0x119, 0x5, 0x1a, 0xe, 0x2, 0x119, 0x11a, 0x7, 0x5, 0x2, 
       0x2, 0x11a, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x11b, 0x11c, 0x5, 0x20, 0x11, 
       0x2, 0x11c, 0x11d, 0x7, 0x24, 0x2, 0x2, 0x11d, 0x12a, 0x3, 0x2, 0x2, 
       0x2, 0x11e, 0x11f, 0x7, 0xb, 0x2, 0x2, 0x11f, 0x124, 0x5, 0x1e, 0x10, 
       0x2, 0x120, 0x121, 0x7, 0x8, 0x2, 0x2, 0x121, 0x123, 0x5, 0x1e, 0x10, 
       0x2, 0x122, 0x120, 0x3, 0x2, 0x2, 0x2, 0x123, 0x126, 0x3, 0x2, 0x2, 
       0x2, 0x124, 0x122, 0x3, 0x2, 0x2, 0x2, 0x124, 0x125, 0x3, 0x2, 0x2, 
       0x2, 0x125, 0x127, 0x3, 0x2, 0x2, 0x2, 0x126, 0x124, 0x3, 0x2, 0x2, 
       0x2, 0x127, 0x128, 0x7, 0xc, 0x2, 0x2, 0x128, 0x12a, 0x3, 0x2, 0x2, 
       0x2, 0x129, 0x11b, 0x3, 0x2, 0x2, 0x2, 0x129, 0x11e, 0x3, 0x2, 0x2, 
       0x2, 0x12a, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x12b, 0x139, 0x9, 0x2, 0x2, 
       0x2, 0x12c, 0x12d, 0x7, 0xe, 0x2, 0x2, 0x12d, 0x12e, 0x7, 0xb, 0x2, 
       0x2, 0x12e, 0x133, 0x5, 0x20, 0x11, 0x2, 0x12f, 0x130, 0x7, 0x8, 
       0x2, 0x2, 0x130, 0x132, 0x5, 0x20, 0x11, 0x2, 0x131, 0x12f, 0x3, 
       0x2, 0x2, 0x2, 0x132, 0x135, 0x3, 0x2, 0x2, 0x2, 0x133, 0x131, 0x3, 
       0x2, 0x2, 0x2, 0x133, 0x134, 0x3, 0x2, 0x2, 0x2, 0x134, 0x136, 0x3, 
       0x2, 0x2, 0x2, 0x135, 0x133, 0x3, 0x2, 0x2, 0x2, 0x136, 0x137, 0x7, 
       0xc, 0x2, 0x2, 0x137, 0x139, 0x3, 0x2, 0x2, 0x2, 0x138, 0x12b, 0x3, 
       0x2, 0x2, 0x2, 0x138, 0x12c, 0x3, 0x2, 0x2, 0x2, 0x139, 0x21, 0x3, 
       0x2, 0x2, 0x2, 0x26, 0x25, 0x2d, 0x35, 0x42, 0x45, 0x4d, 0x5c, 0x61, 
       0x6c, 0x6f, 0x7d, 0x80, 0x8c, 0x8f, 0x93, 0xa0, 0xa9, 0xb6, 0xb9, 
       0xc2, 0xc6, 0xcb, 0xd1, 0xde, 0xe4, 0xf2, 0xf9, 0x102, 0x105, 0x10b, 
       0x10f, 0x115, 0x124, 0x129, 0x133, 0x138, 
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
