#pragma once

#include "entry_point.hpp"

struct boolean_binary_fragment;
struct float_binary_fragment;
struct integer_binary_fragment;
struct string_binary_fragment;
struct array_binary_fragment;
struct without_binary_fragment;
struct user_binary_fragment;
struct boolean_unary_fragment;
struct float_unary_fragment;
struct integer_unary_fragment;
struct string_unary_fragment;
struct array_unary_fragment;
struct without_unary_fragment;
struct user_unary_fragment;
struct pre_evaluation_fragment;
struct post_evaluation_fragment;

namespace fluenc::code_generation
{
	struct emitter_result
	{
		entry_point entryPoint;
		value_stack_t values;
	};

	struct emitter_context
	{
		const entry_point& entryPoint;

		value_stack_t values;
	};

	struct emitter
	{
		using return_type = std::vector<emitter_result>;
		using context_type = emitter_context;

		emitter(const std::vector<std::string>& function)
			: function_(function)
		{
		}

		std::vector<emitter_result> visit(const binary_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const integral_literal_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const float_literal_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const boolean_literal_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const string_literal_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const character_literal_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const nothing_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const member_access_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const function_call_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const instantiation_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const conditional_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const block_instruction_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const array_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const expansion_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const local_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const unary_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const function_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const global_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const return_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const terminator_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const tuple_sink_node* node, emitter_context context) const;
		std::vector<emitter_result> visit(const struct_node* node, emitter_context context) const;

	private:
		std::vector<std::string> function_;
	};
}
