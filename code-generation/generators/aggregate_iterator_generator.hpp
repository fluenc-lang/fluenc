#pragma once

#include <vector>

#include "base_generator.hpp"
#include "expression.hpp"

namespace fluenc::code_generation::generators
{
	class aggregate_iterator_generator : public base_generator
	{
	public:
		aggregate_iterator_generator(const expression_t& node, const std::vector<const base_value*>& input);

		std::vector<emitter_result> generate(const emitter& visitor, emitter_context context, generation_mode mode)
			const override;

		const base_generator* clone(const entry_point& entryPoint, clone_strategy strategy) const override;

		const base_type* type() const override;

	private:
		fluenc::expression_t node_;

		std::vector<const base_value*> input_;
	};
}
