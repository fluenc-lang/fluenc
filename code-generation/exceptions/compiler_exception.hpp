#pragma once

#include <exception>
#include <memory>

#include "token_info.hpp"

class compiler_exception : public std::exception
{
public:
	compiler_exception(const std::shared_ptr<peg::Ast>& ast);

	size_t row() const;
	size_t column() const;
	size_t length() const;

	std::string file() const;

	virtual std::string message() const = 0;

private:
	std::shared_ptr<peg::Ast> m_ast;
};
