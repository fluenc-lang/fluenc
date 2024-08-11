#pragma once

#include <llvm/IR/Value.h>

#include "entry_point.hpp"
#include "token_info.hpp"

namespace fluenc::code_generation
{
	struct base_value;

	namespace values
	{
		struct user_type_value;
	}

	struct read_proxy
	{
		const entry_point entryPoint;
		const base_value* value;
	};

	struct interop_helper
	{
		static read_proxy create_read_proxy(
			llvm::Value* value,
			const base_type* type,
			const entry_point& entryPoint,
			const std::shared_ptr<peg::Ast>& ast
		);

		static llvm::Value* create_write_proxy(
			const values::user_type_value* userTypeValue,
			const entry_point& entryPoint
		);
	};
}
