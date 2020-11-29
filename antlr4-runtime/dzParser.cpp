
// Generated from dz.g4 by ANTLR 4.8


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

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(21); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(20);
      function();
      setState(23); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == dzParser::ID);
   
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

tree::TerminalNode* dzParser::FunctionContext::ID() {
  return getToken(dzParser::ID, 0);
}

dzParser::BlockContext* dzParser::FunctionContext::block() {
  return getRuleContext<dzParser::BlockContext>(0);
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
  enterRule(_localctx, 2, dzParser::RuleFunction);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(25);
    typeName();
    setState(26);
    match(dzParser::ID);
    setState(27);
    match(dzParser::T__0);
    setState(36);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == dzParser::ID) {
      setState(28);
      argument();
      setState(33);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == dzParser::T__1) {
        setState(29);
        match(dzParser::T__1);
        setState(30);
        argument();
        setState(35);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(38);
    match(dzParser::T__2);
    setState(39);
    block();
   
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

tree::TerminalNode* dzParser::ConstantContext::INT() {
  return getToken(dzParser::INT, 0);
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

tree::TerminalNode* dzParser::MemberContext::ID() {
  return getToken(dzParser::ID, 0);
}

dzParser::MemberContext::MemberContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any dzParser::MemberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitMember(this);
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
  size_t startState = 4;
  enterRecursionRule(_localctx, 4, dzParser::RuleExpression, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(57);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<CallContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(42);
      match(dzParser::ID);
      setState(43);
      match(dzParser::T__0);
      setState(52);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == dzParser::INT

      || _la == dzParser::ID) {
        setState(44);
        expression(0);
        setState(49);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == dzParser::T__1) {
          setState(45);
          match(dzParser::T__1);
          setState(46);
          expression(0);
          setState(51);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(54);
      match(dzParser::T__2);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<ConstantContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(55);
      match(dzParser::INT);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MemberContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(56);
      match(dzParser::ID);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(64);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        auto newContext = _tracker.createInstance<BinaryContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
        _localctx = newContext;
        newContext->left = previousContext;
        pushNewRecursionContext(newContext, startState, RuleExpression);
        setState(59);

        if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
        setState(60);
        dynamic_cast<BinaryContext *>(_localctx)->op = match(dzParser::OP);
        setState(61);
        dynamic_cast<BinaryContext *>(_localctx)->right = expression(3); 
      }
      setState(66);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx);
    }
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

tree::TerminalNode* dzParser::AssignmentContext::ID() {
  return getToken(dzParser::ID, 0);
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

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(67);
    match(dzParser::T__3);
    setState(68);
    match(dzParser::ID);
    setState(69);
    match(dzParser::T__4);
    setState(70);
    expression(0);
   
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
  enterRule(_localctx, 8, dzParser::RuleRet);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(72);
    match(dzParser::T__5);
    setState(73);
    expression(0);
   
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
  enterRule(_localctx, 10, dzParser::RuleConditional);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(75);
    match(dzParser::T__6);
    setState(76);
    match(dzParser::T__0);
    setState(77);
    expression(0);
    setState(78);
    match(dzParser::T__2);
    setState(79);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

dzParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

dzParser::AssignmentContext* dzParser::StatementContext::assignment() {
  return getRuleContext<dzParser::AssignmentContext>(0);
}

dzParser::ConditionalContext* dzParser::StatementContext::conditional() {
  return getRuleContext<dzParser::ConditionalContext>(0);
}


size_t dzParser::StatementContext::getRuleIndex() const {
  return dzParser::RuleStatement;
}


antlrcpp::Any dzParser::StatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<dzVisitor*>(visitor))
    return parserVisitor->visitStatement(this);
  else
    return visitor->visitChildren(this);
}

dzParser::StatementContext* dzParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 12, dzParser::RuleStatement);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(83);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case dzParser::T__3: {
        enterOuterAlt(_localctx, 1);
        setState(81);
        assignment();
        break;
      }

      case dzParser::T__6: {
        enterOuterAlt(_localctx, 2);
        setState(82);
        conditional();
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

//----------------- BlockContext ------------------------------------------------------------------

dzParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

dzParser::RetContext* dzParser::BlockContext::ret() {
  return getRuleContext<dzParser::RetContext>(0);
}

std::vector<dzParser::StatementContext *> dzParser::BlockContext::statement() {
  return getRuleContexts<dzParser::StatementContext>();
}

dzParser::StatementContext* dzParser::BlockContext::statement(size_t i) {
  return getRuleContext<dzParser::StatementContext>(i);
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
  enterRule(_localctx, 14, dzParser::RuleBlock);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(85);
    match(dzParser::T__7);
    setState(89);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == dzParser::T__3

    || _la == dzParser::T__6) {
      setState(86);
      statement();
      setState(91);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(92);
    ret();
    setState(93);
    match(dzParser::T__8);
   
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
  enterRule(_localctx, 16, dzParser::RuleArgument);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(95);
    typeName();
    setState(96);
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
  enterRule(_localctx, 18, dzParser::RuleTypeName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(98);
    match(dzParser::ID);
   
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
    case 2: return expressionSempred(dynamic_cast<ExpressionContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool dzParser::expressionSempred(ExpressionContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 2);

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
  "program", "function", "expression", "assignment", "ret", "conditional", 
  "statement", "block", "argument", "typeName"
};

std::vector<std::string> dzParser::_literalNames = {
  "", "'('", "','", "')'", "'var'", "'='", "'return'", "'if'", "'{'", "'}'"
};

std::vector<std::string> dzParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "INT", "OP", "WS", "ID"
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

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0xf, 0x67, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9, 
    0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 0x4, 
    0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 0x9, 
    0xb, 0x3, 0x2, 0x6, 0x2, 0x18, 0xa, 0x2, 0xd, 0x2, 0xe, 0x2, 0x19, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x7, 0x3, 0x22, 
    0xa, 0x3, 0xc, 0x3, 0xe, 0x3, 0x25, 0xb, 0x3, 0x5, 0x3, 0x27, 0xa, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x7, 0x4, 0x32, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x35, 
    0xb, 0x4, 0x5, 0x4, 0x37, 0xa, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 
    0x4, 0x3c, 0xa, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x7, 0x4, 0x41, 0xa, 
    0x4, 0xc, 0x4, 0xe, 0x4, 0x44, 0xb, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 
    0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 0x3, 0x7, 
    0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x8, 0x3, 0x8, 0x5, 0x8, 
    0x56, 0xa, 0x8, 0x3, 0x9, 0x3, 0x9, 0x7, 0x9, 0x5a, 0xa, 0x9, 0xc, 0x9, 
    0xe, 0x9, 0x5d, 0xb, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0xa, 0x3, 
    0xa, 0x3, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x2, 0x3, 0x6, 0xc, 0x2, 
    0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x2, 0x2, 0x2, 0x66, 
    0x2, 0x17, 0x3, 0x2, 0x2, 0x2, 0x4, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x6, 0x3b, 
    0x3, 0x2, 0x2, 0x2, 0x8, 0x45, 0x3, 0x2, 0x2, 0x2, 0xa, 0x4a, 0x3, 0x2, 
    0x2, 0x2, 0xc, 0x4d, 0x3, 0x2, 0x2, 0x2, 0xe, 0x55, 0x3, 0x2, 0x2, 0x2, 
    0x10, 0x57, 0x3, 0x2, 0x2, 0x2, 0x12, 0x61, 0x3, 0x2, 0x2, 0x2, 0x14, 
    0x64, 0x3, 0x2, 0x2, 0x2, 0x16, 0x18, 0x5, 0x4, 0x3, 0x2, 0x17, 0x16, 
    0x3, 0x2, 0x2, 0x2, 0x18, 0x19, 0x3, 0x2, 0x2, 0x2, 0x19, 0x17, 0x3, 
    0x2, 0x2, 0x2, 0x19, 0x1a, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x3, 0x3, 0x2, 
    0x2, 0x2, 0x1b, 0x1c, 0x5, 0x14, 0xb, 0x2, 0x1c, 0x1d, 0x7, 0xf, 0x2, 
    0x2, 0x1d, 0x26, 0x7, 0x3, 0x2, 0x2, 0x1e, 0x23, 0x5, 0x12, 0xa, 0x2, 
    0x1f, 0x20, 0x7, 0x4, 0x2, 0x2, 0x20, 0x22, 0x5, 0x12, 0xa, 0x2, 0x21, 
    0x1f, 0x3, 0x2, 0x2, 0x2, 0x22, 0x25, 0x3, 0x2, 0x2, 0x2, 0x23, 0x21, 
    0x3, 0x2, 0x2, 0x2, 0x23, 0x24, 0x3, 0x2, 0x2, 0x2, 0x24, 0x27, 0x3, 
    0x2, 0x2, 0x2, 0x25, 0x23, 0x3, 0x2, 0x2, 0x2, 0x26, 0x1e, 0x3, 0x2, 
    0x2, 0x2, 0x26, 0x27, 0x3, 0x2, 0x2, 0x2, 0x27, 0x28, 0x3, 0x2, 0x2, 
    0x2, 0x28, 0x29, 0x7, 0x5, 0x2, 0x2, 0x29, 0x2a, 0x5, 0x10, 0x9, 0x2, 
    0x2a, 0x5, 0x3, 0x2, 0x2, 0x2, 0x2b, 0x2c, 0x8, 0x4, 0x1, 0x2, 0x2c, 
    0x2d, 0x7, 0xf, 0x2, 0x2, 0x2d, 0x36, 0x7, 0x3, 0x2, 0x2, 0x2e, 0x33, 
    0x5, 0x6, 0x4, 0x2, 0x2f, 0x30, 0x7, 0x4, 0x2, 0x2, 0x30, 0x32, 0x5, 
    0x6, 0x4, 0x2, 0x31, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x32, 0x35, 0x3, 0x2, 
    0x2, 0x2, 0x33, 0x31, 0x3, 0x2, 0x2, 0x2, 0x33, 0x34, 0x3, 0x2, 0x2, 
    0x2, 0x34, 0x37, 0x3, 0x2, 0x2, 0x2, 0x35, 0x33, 0x3, 0x2, 0x2, 0x2, 
    0x36, 0x2e, 0x3, 0x2, 0x2, 0x2, 0x36, 0x37, 0x3, 0x2, 0x2, 0x2, 0x37, 
    0x38, 0x3, 0x2, 0x2, 0x2, 0x38, 0x3c, 0x7, 0x5, 0x2, 0x2, 0x39, 0x3c, 
    0x7, 0xc, 0x2, 0x2, 0x3a, 0x3c, 0x7, 0xf, 0x2, 0x2, 0x3b, 0x2b, 0x3, 
    0x2, 0x2, 0x2, 0x3b, 0x39, 0x3, 0x2, 0x2, 0x2, 0x3b, 0x3a, 0x3, 0x2, 
    0x2, 0x2, 0x3c, 0x42, 0x3, 0x2, 0x2, 0x2, 0x3d, 0x3e, 0xc, 0x4, 0x2, 
    0x2, 0x3e, 0x3f, 0x7, 0xd, 0x2, 0x2, 0x3f, 0x41, 0x5, 0x6, 0x4, 0x5, 
    0x40, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x41, 0x44, 0x3, 0x2, 0x2, 0x2, 0x42, 
    0x40, 0x3, 0x2, 0x2, 0x2, 0x42, 0x43, 0x3, 0x2, 0x2, 0x2, 0x43, 0x7, 
    0x3, 0x2, 0x2, 0x2, 0x44, 0x42, 0x3, 0x2, 0x2, 0x2, 0x45, 0x46, 0x7, 
    0x6, 0x2, 0x2, 0x46, 0x47, 0x7, 0xf, 0x2, 0x2, 0x47, 0x48, 0x7, 0x7, 
    0x2, 0x2, 0x48, 0x49, 0x5, 0x6, 0x4, 0x2, 0x49, 0x9, 0x3, 0x2, 0x2, 
    0x2, 0x4a, 0x4b, 0x7, 0x8, 0x2, 0x2, 0x4b, 0x4c, 0x5, 0x6, 0x4, 0x2, 
    0x4c, 0xb, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x4e, 0x7, 0x9, 0x2, 0x2, 0x4e, 
    0x4f, 0x7, 0x3, 0x2, 0x2, 0x4f, 0x50, 0x5, 0x6, 0x4, 0x2, 0x50, 0x51, 
    0x7, 0x5, 0x2, 0x2, 0x51, 0x52, 0x5, 0x10, 0x9, 0x2, 0x52, 0xd, 0x3, 
    0x2, 0x2, 0x2, 0x53, 0x56, 0x5, 0x8, 0x5, 0x2, 0x54, 0x56, 0x5, 0xc, 
    0x7, 0x2, 0x55, 0x53, 0x3, 0x2, 0x2, 0x2, 0x55, 0x54, 0x3, 0x2, 0x2, 
    0x2, 0x56, 0xf, 0x3, 0x2, 0x2, 0x2, 0x57, 0x5b, 0x7, 0xa, 0x2, 0x2, 
    0x58, 0x5a, 0x5, 0xe, 0x8, 0x2, 0x59, 0x58, 0x3, 0x2, 0x2, 0x2, 0x5a, 
    0x5d, 0x3, 0x2, 0x2, 0x2, 0x5b, 0x59, 0x3, 0x2, 0x2, 0x2, 0x5b, 0x5c, 
    0x3, 0x2, 0x2, 0x2, 0x5c, 0x5e, 0x3, 0x2, 0x2, 0x2, 0x5d, 0x5b, 0x3, 
    0x2, 0x2, 0x2, 0x5e, 0x5f, 0x5, 0xa, 0x6, 0x2, 0x5f, 0x60, 0x7, 0xb, 
    0x2, 0x2, 0x60, 0x11, 0x3, 0x2, 0x2, 0x2, 0x61, 0x62, 0x5, 0x14, 0xb, 
    0x2, 0x62, 0x63, 0x7, 0xf, 0x2, 0x2, 0x63, 0x13, 0x3, 0x2, 0x2, 0x2, 
    0x64, 0x65, 0x7, 0xf, 0x2, 0x2, 0x65, 0x15, 0x3, 0x2, 0x2, 0x2, 0xb, 
    0x19, 0x23, 0x26, 0x33, 0x36, 0x3b, 0x42, 0x55, 0x5b, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

dzParser::Initializer dzParser::_init;
