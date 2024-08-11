#pragma once

#include <string>

#include "compiler_exception.hpp"

class missing_field_exception : public compiler_exception
{
public:
	missing_field_exception(
		const std::shared_ptr<peg::Ast>& ast,
		const std::string& typeName,
		const std::string& fieldName
	);

	std::string message() const override;

private:
	std::string type_name_;
	std::string field_name_;
};
