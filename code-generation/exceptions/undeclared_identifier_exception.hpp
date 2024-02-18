#pragma once

#include <string>

#include "compiler_exception.hpp"

class undeclared_identifier_exception : public compiler_exception
{
public:
	undeclared_identifier_exception(const std::shared_ptr<peg::Ast>& ast, const std::string& identifier);

	std::string message() const override;

private:
	std::string identifier_;
};
