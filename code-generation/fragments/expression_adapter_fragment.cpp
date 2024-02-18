#include "expression_adapter_fragment.hpp"
#include "ast.hpp"
#include "emitter.hpp"

namespace fluenc::code_generation::fragments
{
	expression_adapter_fragment::expression_adapter_fragment(const expression_t& subject)
		: subject_(subject)
	{
	}

	std::vector<emitter_result> expression_adapter_fragment::build(const emitter& visitor, emitter_context context) const
	{
		return accept(subject_, visitor, context);
	}
}
