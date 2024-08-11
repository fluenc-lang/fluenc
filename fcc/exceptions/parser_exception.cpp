#include <vector>

#include <peglib.h>

#include "exceptions/parser_exception.hpp"

parser_exception::parser_exception(const std::string& file, size_t line, size_t col, const std::string& message)
	: compiler_exception(
		  std::make_shared<peg::Ast>(file.c_str(), line, col, "dummy", std::vector<std::shared_ptr<peg::Ast>>())
	  )
	, message_(message)
{
}

std::string parser_exception::message() const
{
	return message_;
}
