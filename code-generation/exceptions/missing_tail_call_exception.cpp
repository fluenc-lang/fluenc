#include "missing_tail_call_exception.hpp"

missing_tail_call_exception::missing_tail_call_exception(const std::shared_ptr<peg::Ast>& ast)
	: compiler_exception(ast)
{
}

std::string missing_tail_call_exception::message() const
{
	return "Tail call expected. Use tail keyword.";
}
