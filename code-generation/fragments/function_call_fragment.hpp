#pragma once

#include "fragment.hpp"
#include "token_info.hpp"

#include <memory>
#include <string>

namespace fluenc::code_generation::fragments
{
	struct function_call_fragment : public fragment
	{
		function_call_fragment(const std::vector<std::string>& names, const std::shared_ptr<peg::Ast>& ast);

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

	private:
		std::vector<std::string> names_;
		std::shared_ptr<peg::Ast> ast_;
	};
}
