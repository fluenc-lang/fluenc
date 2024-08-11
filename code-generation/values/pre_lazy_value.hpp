#pragma once

#include "base_value.hpp"
#include "emitter.hpp"

#include "metadata/lazy_value_metadata.hpp"

namespace fluenc::code_generation
{
	namespace generators
	{
		struct base_generator;
	}

	struct fragment;
	struct emitter;

	namespace values
	{
		struct pre_lazy_value : public base_value_with_metadata<lazy_value_metadata>
		{
			pre_lazy_value(const generators::base_generator* generator, const entry_point& entryPoint);

			value_id id() const override;

			const base_type* type() const override;

			const base_value* clone(const entry_point& entry_point, clone_strategy strategy) const override;

			std::vector<emitter_result> generate(const emitter& visitor, emitter_context context) const;

			entry_point assign_from(const entry_point& entryPoint, const base_value* source, const emitter& emitter) const;
			entry_point assign_from(const entry_point& entryPoint, const pre_lazy_value* source, const emitter& emitter)
				const;

		private:
			const generators::base_generator* generator_;
			const entry_point* entry_point_;

			mutable const base_type* type_;
		};
	}
}
