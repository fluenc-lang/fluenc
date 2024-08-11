#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "token_info.hpp"

struct default_type_name;
struct function_type_name;

using type_name_t = std::variant<const default_type_name*, const function_type_name*>;

struct default_type_name
{
	static type_name_t int32();
	static type_name_t int64();
	static type_name_t f32();
	static type_name_t uint32();
	static type_name_t byte();
	static type_name_t boolean();
	static type_name_t string();
	static type_name_t without();

	std::shared_ptr<peg::Ast> ast;
	std::vector<std::string> names;
};

struct function_type_name
{
	std::vector<type_name_t> types;
};
