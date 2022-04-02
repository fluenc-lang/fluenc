#include "CompilerException.h"

TokenInfo createTokenInfo(ParserRuleContext *context)
{
	return
	{
		context->start->getLine(),
		context->start->getCharPositionInLine(),
		context->start->getStopIndex() - context->start->getStartIndex(),
	};
}

CompilerException::CompilerException(ParserRuleContext *context)
	: m_tokenInfo(createTokenInfo(context))
{
}

size_t CompilerException::row() const
{
	return m_tokenInfo.row;
}

size_t CompilerException::column() const
{
	return m_tokenInfo.column;
}

size_t CompilerException::length() const
{
	return m_tokenInfo.length;
}
