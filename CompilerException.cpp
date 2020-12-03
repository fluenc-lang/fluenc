#include "CompilerException.h"

CompilerException::CompilerException(ParserRuleContext *context)
	: m_context(context)
{
}

int CompilerException::row() const
{
	return m_context->start->getLine();
}

int CompilerException::column() const
{
	return m_context->start->getCharPositionInLine();
}

int CompilerException::length() const
{
	return m_context->start->getStopIndex() - m_context->start->getStartIndex();
}
