#pragma once

#include "DzResult.h"
#include "Stack.h"

class BaseValue;

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
struct junction_node;
struct pre_evaluation_node;
struct post_evaluation_node;
struct stack_segment_node;

class ArrayElementNode;
class StaticNode;
class ArrayContinuationNode;
class ArrayValue;
class RenderedNode;
class DefaultNode;
class IteratorValue;
class StringIteratable;

class Emitter
{
	public:
		using return_type = std::vector<DzResult>;
		using context_type = DefaultVisitorContext;

		std::vector<DzResult> visit(const boolean_binary_fragment *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const float_binary_fragment* node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const integer_binary_fragment* node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const string_binary_fragment* node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const array_binary_fragment* node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const without_binary_fragment* node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const user_binary_fragment* node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::binary_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const ArrayContinuationNode *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const ArrayElementNode *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::integral_literal_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::float_literal_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::boolean_literal_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::string_literal_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::character_literal_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::nothing_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::member_access_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::reference_sink_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const pre_evaluation_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const post_evaluation_node *node, DefaultVisitorContext context) const;
		// std::vector<DzResult> visit(const fluenc::function_call_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const stack_segment_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::function_call_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const junction_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::instantiation_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::conditional_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::block_instruction_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::empty_array_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::index_sink_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::array_sink_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::expansion_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::local_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const boolean_unary_fragment *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const float_unary_fragment *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const integer_unary_fragment *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const string_unary_fragment *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const array_unary_fragment *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const without_unary_fragment *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const user_unary_fragment *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::unary_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::tail_function_call_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::function_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::global_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::return_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::terminator_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const ArrayValue *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const IteratorValue *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const StringIteratable *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::tuple_sink_node *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const DefaultNode *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const StaticNode *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const RenderedNode *node, DefaultVisitorContext context) const;
		std::vector<DzResult> visit(const fluenc::extension_node *node, DefaultVisitorContext context) const;

		std::vector<DzResult> make_function_call(const std::shared_ptr<peg::Ast> &ast, const std::vector<std::string>& names, DefaultVisitorContext context) const;

	private:
		std::vector<std::string> function_;
};
