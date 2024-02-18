#pragma once

#include <string>

#include "compiler_exception.hpp"

class missing_default_value_exception : public compiler_exception
{
public:
	missing_default_value_exception(const std::shared_ptr<peg::Ast>& ast, const std::string& fieldName);

	std::string message() const override;

private:
	std::string field_name_;
};
