#pragma once

#include <variant>

namespace fluenc
{
	struct global_node;
	struct array_sink_node;
	struct conditional_node;
	struct float_literal_node;
	struct nothing_node;
	struct reference_sink_node;
	struct empty_array_node;
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
	struct index_sink_node;
	struct tuple_sink_node;
	struct binary_node;
	struct unary_node;
	struct tail_function_call_node;
	struct function_call_node;
	struct block_instruction_node;
	struct function_node;
	struct extension_node;

	using expression_t = std::variant<
		const fluenc::array_sink_node*,
		const fluenc::binary_node*,
		const fluenc::float_literal_node*,
		const fluenc::block_instruction_node*,
		const fluenc::boolean_literal_node*,
		const fluenc::character_literal_node*,
		const fluenc::conditional_node*,
		const fluenc::empty_array_node*,
		const fluenc::expansion_node*,
		const fluenc::function_call_node*,
		const fluenc::index_sink_node*,
		const fluenc::instantiation_node*,
		const fluenc::integral_literal_node*,
		const fluenc::local_node*,
		const fluenc::member_access_node*,
		const fluenc::nothing_node*,
		const fluenc::reference_sink_node*,
		const fluenc::return_node*,
		const fluenc::string_literal_node*,
		const fluenc::terminator_node*,
		const fluenc::unary_node*,
		const fluenc::tail_function_call_node*,
		const fluenc::global_node*,
		const fluenc::function_node*,
		const fluenc::tuple_sink_node*,
		const fluenc::extension_node*>;
}
