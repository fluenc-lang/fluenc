#pragma once

#include "fragment.hpp"

namespace fluenc::code_generation
{
	namespace values
	{
		struct reference_value;
	}

	namespace fragments
	{
		struct index_injector_fragment : public fragment
		{
			index_injector_fragment(const values::reference_value* index, const fragment* subject);

			std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

		private:
			const values::reference_value* index_;
			const fragment* subject_;
		};
	}
}
