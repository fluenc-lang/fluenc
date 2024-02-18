#pragma once

#include "compiler_exception.hpp"

#include <vector>

namespace fluenc
{
	struct function_node;

	namespace code_generation
	{
		struct base_type;

		namespace exceptions
		{
			using tried_t = std::vector<std::pair<int8_t, const fluenc::function_node*>>;

			class function_not_found_exception : public compiler_exception
			{
			public:
				function_not_found_exception(
					const std::shared_ptr<peg::Ast>& ast,
					const std::string& name,
					const std::vector<const base_type*>& values,
					const tried_t& tried
				);

				std::string message() const override;

			private:
				std::string name_;
				std::vector<const base_type*> values_;

				tried_t tried_;
			};
		}
	}
}
