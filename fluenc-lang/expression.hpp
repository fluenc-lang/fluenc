#pragma once

#include <variant>

namespace fluenc
{
	struct global_node;
	struct conditional_node;
	struct float_literal_node;
	struct nothing_node;
	struct terminator_node;
	struct expansion_node;
	struct local_node;
	struct character_literal_node;
	struct string_literal_node;
	struct boolean_literal_node;
	struct integral_literal_node;
	struct member_access_node;
	struct instantiation_node;
	struct return_node;
	struct tuple_sink_node;
	struct binary_node;
	struct unary_node;
	struct function_call_node;
	struct block_instruction_node;
	struct function_node;
	struct array_node;

	using expression_t = std::variant<
		const binary_node*,
		const float_literal_node*,
		const block_instruction_node*,
		const boolean_literal_node*,
		const character_literal_node*,
		const conditional_node*,
		const expansion_node*,
		const function_call_node*,
		const instantiation_node*,
		const integral_literal_node*,
		const local_node*,
		const member_access_node*,
		const nothing_node*,
		const return_node*,
		const string_literal_node*,
		const terminator_node*,
		const unary_node*,
		const global_node*,
		const function_node*,
		const tuple_sink_node*,
		const array_node*>;
}
