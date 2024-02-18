#pragma once

#include "compiler_exception.hpp"

class invalid_argument_type_exception : public compiler_exception
{
public:
	invalid_argument_type_exception(const std::shared_ptr<peg::Ast>& ast);

	std::string message() const override;
};
