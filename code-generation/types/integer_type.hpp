#pragma once

#include "builtin_type.hpp"
#include "integer_operator_set.hpp"

namespace fluenc::code_generation::types
{
	template <typename T>
	struct integer_type : public builtin_type<T>
	{
		operator_set* operators() const override
		{
			static integer_operator_set operators;

			return &operators;
		}
	};
}
