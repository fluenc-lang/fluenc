#pragma once

#include <string>

#include "compiler_exception.hpp"

class invalid_function_pointer_type_exception : public compiler_exception
{
public:
	invalid_function_pointer_type_exception(const std::shared_ptr<peg::Ast>& ast, const std::string& name);

	std::string message() const override;

private:
	std::string name_;
};
