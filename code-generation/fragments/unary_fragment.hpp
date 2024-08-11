#pragma once

#include "ast/unary_node.hpp"

#include "emitter.hpp"
#include "fragment.hpp"
#include "utility.hpp"

namespace fluenc::code_generation::fragments
{
	struct integer_unary_fragment : public fragment
	{
		integer_unary_fragment(const fluenc::unary_node* unary)
			: unary_(unary)
		{
		}

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

	private:
		const fluenc::unary_node* unary_;
	};

	struct boolean_unary_fragment : public fragment
	{
		boolean_unary_fragment(const fluenc::unary_node* unary)
			: unary_(unary)
		{
		}

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

	private:
		const fluenc::unary_node* unary_;
	};

	struct float_unary_fragment : public fragment
	{
		float_unary_fragment(const fluenc::unary_node* unary)
			: unary_(unary)
		{
		}

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

	private:
		const fluenc::unary_node* unary_;
	};

	struct string_unary_fragment : public fragment
	{
		string_unary_fragment(const fluenc::unary_node* unary)
			: unary_(unary)
		{
		}

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

	private:
		const fluenc::unary_node* unary_;
	};

	struct array_unary_fragment : public fragment
	{
		array_unary_fragment(const fluenc::unary_node* unary)
			: unary_(unary)
		{
		}

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

	private:
		const fluenc::unary_node* unary_;
	};

	struct without_unary_fragment : public fragment
	{
		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override
		{
			UNUSED(visitor);

			return { { context.entryPoint, context.values } };
		}
	};

	struct user_unary_fragment : public fragment
	{
		user_unary_fragment(const fluenc::unary_node* unary)
			: unary_(unary)
		{
		}

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

	private:
		const fluenc::unary_node* unary_;
	};
}
