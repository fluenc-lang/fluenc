#pragma once

#include <vector>

#include "base_generator.hpp"

namespace fluenc::code_generation
{
	namespace fragments
	{
		struct stack_segment_fragment;
	}

	namespace generators
	{
		struct iterator_generator : public base_generator
		{
			struct create_new
			{
				const fragments::stack_segment_fragment* node;

				entry_point* ep;
			};

			struct use_rendered
			{
				std::vector<emitter_result> results;

				entry_point* ep;

				create_new next;
			};

			using strategy_t = std::variant<use_rendered, create_new>;

		public:
			iterator_generator(
				const base_type* type,
				const std::vector<emitter_result>& results,
				entry_point* preliminary_entry_point,
				fragments::stack_segment_fragment* subject,
				const entry_point& captured_entry_point
			);

			std::vector<emitter_result> generate(const emitter& visitor, emitter_context context, generation_mode mode)
				const override;

			const base_generator* clone(const entry_point& entryPoint, clone_strategy strategy) const override;
			const base_type* type() const override;

		private:
			const base_type* type_;

			std::vector<emitter_result> results_;

			mutable strategy_t strategy_;
		};
	}
}
