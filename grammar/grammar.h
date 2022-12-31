#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <incbin.h>

#include <string_view>

INCBIN_EXTERN(Grammar);

std::string_view grammar()
{
	return { reinterpret_cast<const char *>(gGrammarData), gGrammarSize };
}

#endif // GRAMMAR_H
