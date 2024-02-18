#pragma once

#include "exceptions/compiler_exception.hpp"

#include <string>

struct parser_exception : public compiler_exception
{
	parser_exception(const std::string& file, size_t line, size_t col, const std::string& message);

	std::string message() const override;

private:
	std::string message_;
};
