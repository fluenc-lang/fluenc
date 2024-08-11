#pragma once

#include <string>

#include "compiler_exception.hpp"

class invalid_type_exception : public compiler_exception
{
public:
	invalid_type_exception(
		const std::shared_ptr<peg::Ast>& ast,
		const std::string& expected_type,
		const std::string& actual_type
	);

	std::string message() const override;

private:
	std::shared_ptr<peg::Ast> ast_;
	std::string expected_type_;
	std::string actual_type_;
};
