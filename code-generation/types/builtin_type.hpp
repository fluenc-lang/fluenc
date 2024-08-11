#pragma once

#include "type.hpp"

namespace fluenc::code_generation::types
{
	struct builtin_base_type : public base_type
	{
	};

	template <typename T>
	struct builtin_type : public builtin_base_type
	{
		type_id id() const override
		{
			return type_id::builtin;
		}

		static base_type* instance()
		{
			static T instance;

			return &instance;
		}
	};
}
