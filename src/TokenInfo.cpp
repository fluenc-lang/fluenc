#include "TokenInfo.h"

TokenInfo TokenInfo::fromContext(ParserRuleContext *context)
{
	if (!context)
	{
		return TokenInfo();
	}

	return
	{
		context->start->getLine(),
		context->start->getCharPositionInLine(),
		context->start->getStopIndex() - context->start->getStartIndex(),
	};
}
