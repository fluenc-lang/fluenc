#pragma once

#include <string>

#include "compiler_exception.hpp"

class binary_type_mismatch_exception : public compiler_exception
{
public:
	binary_type_mismatch_exception(
		const std::shared_ptr<peg::Ast>& ast,
		const std::string& leftTypeName,
		const std::string& rightTypeName
	);

	std::string message() const override;

private:
	std::string left_type_name_;
	std::string right_type_name_;
};
