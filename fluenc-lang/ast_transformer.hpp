#pragma once

#include <any>
#include <memory>
#include <vector>

#include "ast/argument_node.hpp"
#include "ast/struct_field_node.hpp"

namespace fluenc
{
	struct module_node;
	struct namespace_t;
	struct global_node;
	struct struct_node;

	struct ast_transformer
	{
		ast_transformer(
			const std::vector<std::string>& namespaces,
			const std::optional<uint64_t>& iterator,
			const std::optional<expression_t>& parent,
			const std::optional<expression_t>& alpha,
			const std::optional<expression_t>& beta
		);

		module_node transform(const std::shared_ptr<peg::Ast>& ast) const;

	private:
		std::any visit_instruction(const std::shared_ptr<peg::Ast>& ast) const;

		std::string visit_id(const std::shared_ptr<peg::Ast>& ast) const;
		std::string visit_integer(const std::shared_ptr<peg::Ast>& ast) const;
		std::string visit_string(const std::shared_ptr<peg::Ast>& ast) const;

		std::vector<type_name_t> visit_type_list(const std::shared_ptr<peg::Ast>& ast) const;
		std::vector<struct_field_t> visit_field_list(const std::shared_ptr<peg::Ast>& ast) const;
		std::vector<std::string> visit_id_list(const std::shared_ptr<peg::Ast>& ast) const;

		expression_t visit_expression(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_literal(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_int32_literal(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_int64_literal(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_float32_literal(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_boolean_literal(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_string_literal(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_uint32_literal(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_char_literal(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_byte_literal(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_nothing_literal(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_binary(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_member(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_call(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_instantiation(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_conditional(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_array(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_tuple(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_group(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_expansion(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_local(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_continuation(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_with(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visit_unary(const std::shared_ptr<peg::Ast>& ast) const;

		function_node* visit_function(const std::shared_ptr<peg::Ast>& ast) const;
		function_node* visit_regular_function(const std::shared_ptr<peg::Ast>& ast) const;
		function_node* visit_exported_function(const std::shared_ptr<peg::Ast>& ast) const;
		function_node* visit_imported_function(const std::shared_ptr<peg::Ast>& ast) const;

		argument_t visit_argument(const std::shared_ptr<peg::Ast>& ast) const;
		argument_t visit_standard_argument(const std::shared_ptr<peg::Ast>& ast) const;
		argument_t visit_tuple_argument(const std::shared_ptr<peg::Ast>& ast) const;

		struct_node* visit_structure(const std::shared_ptr<peg::Ast>& ast) const;
		global_node* visit_global(const std::shared_ptr<peg::Ast>& ast) const;
		namespace_t* visit_namespace(const std::shared_ptr<peg::Ast>& ast) const;

		block_instruction_node* visit_return(const std::shared_ptr<peg::Ast>& ast) const;
		block_instruction_node* visit_block(const std::shared_ptr<peg::Ast>& ast) const;

		type_name_t visit_type_name(const std::shared_ptr<peg::Ast>& ast) const;
		type_name_t visit_regular_type(const std::shared_ptr<peg::Ast>& ast) const;
		type_name_t visit_function_type(const std::shared_ptr<peg::Ast>& ast) const;

		struct_field_t visit_field(const std::shared_ptr<peg::Ast>& ast) const;
		struct_field_t visit_standard_field(const std::shared_ptr<peg::Ast>& ast) const;
		struct_field_t visit_decorated_field(const std::shared_ptr<peg::Ast>& ast) const;

		std::vector<std::string> namespaces_;

		std::optional<uint64_t> iterator_;

		std::optional<expression_t> parent_;
		std::optional<expression_t> alpha_;
		std::optional<expression_t> beta_;
	};
}
