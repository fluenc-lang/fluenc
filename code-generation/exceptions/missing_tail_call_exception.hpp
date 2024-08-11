#pragma once

#include <string>

#include "compiler_exception.hpp"

class missing_tail_call_exception : public compiler_exception
{
public:
	missing_tail_call_exception(const std::shared_ptr<peg::Ast>& ast);

	std::string message() const override;
};
