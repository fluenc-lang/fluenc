#pragma once

#include "base_generator.hpp"

#include <llvm/IR/Value.h>

namespace fluenc::code_generation
{
	namespace values
	{
		struct reference_value;
	}

	namespace generators
	{
		class string_iterator_generator : public base_generator
		{
		public:
			string_iterator_generator(const values::reference_value* address, const values::reference_value* length);

			std::vector<emitter_result> generate(const emitter& visitor, emitter_context context, generation_mode mode)
				const override;

			const base_generator* clone(const entry_point& entryPoint, clone_strategy strategy) const override;

			const base_type* type() const override;

		private:
			const values::reference_value* address_;
			const values::reference_value* length_;
		};
	}
}
