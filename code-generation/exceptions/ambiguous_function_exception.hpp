#pragma once

#include <vector>

#include "compiler_exception.hpp"

namespace fluenc
{
	struct function_node;

	namespace code_generation
	{
		struct entry_point;

		namespace exceptions
		{
			struct ambiguous_function_exception : public compiler_exception
			{
				ambiguous_function_exception(
					const std::shared_ptr<peg::Ast>& ast,
					const std::vector<fluenc::function_node*>& functions,
					const entry_point& entryPoint
				);

				std::string message() const override;

			private:
				std::vector<fluenc::function_node*> functions_;

				const entry_point* entry_point_;
			};
		}
	}
}
