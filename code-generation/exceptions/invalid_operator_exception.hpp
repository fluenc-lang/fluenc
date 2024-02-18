#pragma once

#include <string>

#include "compiler_exception.hpp"

class invalid_operator_exception : public compiler_exception
{
public:
	invalid_operator_exception(const std::shared_ptr<peg::Ast>& ast, const std::string& op, const std::string& typeName);

	std::string message() const override;

private:
	std::string operator_;
	std::string type_name_;
};
