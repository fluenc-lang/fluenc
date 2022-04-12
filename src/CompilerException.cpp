#include "CompilerException.h"

CompilerException::CompilerException(const TokenInfo &token)
	: m_token(token)
{
}

size_t CompilerException::row() const
{
	return m_token.row;
}

size_t CompilerException::column() const
{
	return m_token.column;
}

size_t CompilerException::length() const
{
	return m_token.length;
}
