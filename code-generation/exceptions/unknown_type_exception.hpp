#pragma once

#include <string>

#include "compiler_exception.hpp"

class unknown_type_exception : public compiler_exception
{
public:
	unknown_type_exception(const std::shared_ptr<peg::Ast>& ast, const std::string& typeName);

	std::string message() const override;

private:
	std::string type_name_;
};
