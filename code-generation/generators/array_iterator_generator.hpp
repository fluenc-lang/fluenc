#pragma once

#include <vector>

#include "base_generator.hpp"
#include "emitter.hpp"

class Node;

namespace fluenc::code_generation::generators
{
	class array_iterator_generator : public base_generator
	{
	public:
		array_iterator_generator(
			const std::vector<emitter_result>& values,
			const std::shared_ptr<peg::Ast>& ast,
			size_t size
		);

		std::vector<emitter_result> generate(const emitter& visitor, emitter_context context, generation_mode mode)
			const override;
		const base_generator* clone(const entry_point& entryPoint, clone_strategy strategy) const override;
		const base_type* type() const override;

	private:
		const std::vector<emitter_result> values_;
		const std::shared_ptr<peg::Ast> ast_;

		size_t size_;
	};
}
