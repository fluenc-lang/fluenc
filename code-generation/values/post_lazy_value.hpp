#pragma once

#include "base_value.hpp"
#include "emitter.hpp"
#include "entry_point.hpp"

#include "metadata/iterator_metadata.hpp"

namespace fluenc::code_generation
{
	struct emitter;
	struct fragment;
	struct emitter_context;

	namespace values
	{
		struct post_lazy_value : public base_value_with_metadata<iterator_metadata>
		{
			struct create_new
			{
				const fragment* node;

				value_stack values;
			};

			struct use_rendered
			{
				std::vector<emitter_result> results;

				entry_point* ep;

				create_new next;
			};

			using strategy_t = std::variant<use_rendered, create_new>;

			post_lazy_value(
				const fragment* node,
				const base_type* type,
				const entry_point& entryPoint,
				const value_stack& values,
				const std::vector<emitter_result>& results
			);

			value_id id() const override;

			const base_type* type() const override;
			const base_value* clone(const entry_point& entryPoint, clone_strategy strategy) const override;

			std::vector<emitter_result> generate(const emitter& emitter, emitter_context context) const;

		private:
			const base_type* type_;

			mutable strategy_t strategy_;
		};
	}
}
