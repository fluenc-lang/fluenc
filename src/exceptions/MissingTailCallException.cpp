#include "exceptions/MissingTailCallException.h"

MissingTailCallException::MissingTailCallException(const std::shared_ptr<peg::Ast> &ast)
	: CompilerException(ast)
{
}

std::string MissingTailCallException::message() const
{
	return "Tail call expected. Use tail keyword.";
}
