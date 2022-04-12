#ifndef TOKENINFO_H
#define TOKENINFO_H

#include "antlr4-runtime/fluencBaseVisitor.h"
#include "antlr4-runtime/fluencLexer.h"
#include "antlr4-runtime/fluencParser.h"

using namespace antlr4;

struct TokenInfo
{
	size_t row;
	size_t column;
	size_t length;

	static TokenInfo fromContext(ParserRuleContext *context);
};

#endif // TOKENINFO_H
