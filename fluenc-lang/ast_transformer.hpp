#pragma once

#include <any>
#include <memory>
#include <vector>

#include "ast/argument_node.hpp"
#include "ast/struct_field_node.hpp"

struct iterator
{
};

namespace fluenc
{
	struct module_node;
	struct namespace_t;
	struct global_node;
	struct struct_node;

	class ast_transformer
	{
	public:
		ast_transformer(
			const std::vector<std::string>& namespaces,
			const iterator* iteratorType,
			const std::optional<expression_t>& parent,
			const std::optional<expression_t>& alpha,
			const std::optional<expression_t>& beta
		);

		module_node transform(const std::shared_ptr<peg::Ast>& ast) const;

	private:
		std::any visitInstruction(const std::shared_ptr<peg::Ast>& ast) const;

		std::string visitId(const std::shared_ptr<peg::Ast>& ast) const;
		std::string visitInteger(const std::shared_ptr<peg::Ast>& ast) const;
		std::string visitString(const std::shared_ptr<peg::Ast>& ast) const;

		std::vector<type_name_t> visitTypeList(const std::shared_ptr<peg::Ast>& ast) const;
		std::vector<struct_field_t> visitFieldList(const std::shared_ptr<peg::Ast>& ast) const;
		std::vector<std::string> visitIdList(const std::shared_ptr<peg::Ast>& ast) const;

		expression_t visitExpression(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitLiteral(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitInt32Literal(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitInt64Literal(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitFloat32Literal(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitBooleanLiteral(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitStringLiteral(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitUint32Literal(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitCharLiteral(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitByteLiteral(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitNothingLiteral(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitBinary(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitMember(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitCall(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitInstantiation(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitConditional(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitArray(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitTuple(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitGroup(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitExpansion(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitLocal(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitContinuation(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitWith(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitUnary(const std::shared_ptr<peg::Ast>& ast) const;
		expression_t visitTail(const std::shared_ptr<peg::Ast>& ast) const;

		function_node* visitFunction(const std::shared_ptr<peg::Ast>& ast) const;
		function_node* visitRegularFunction(const std::shared_ptr<peg::Ast>& ast) const;
		function_node* visitExportedFunction(const std::shared_ptr<peg::Ast>& ast) const;
		function_node* visitImportedFunction(const std::shared_ptr<peg::Ast>& ast) const;

		argument_t visitArgument(const std::shared_ptr<peg::Ast>& ast) const;
		argument_t visitStandardArgument(const std::shared_ptr<peg::Ast>& ast) const;
		argument_t visitTupleArgument(const std::shared_ptr<peg::Ast>& ast) const;

		struct_node* visitStructure(const std::shared_ptr<peg::Ast>& ast) const;
		global_node* visitGlobal(const std::shared_ptr<peg::Ast>& ast) const;
		namespace_t* visitNamespace(const std::shared_ptr<peg::Ast>& ast) const;

		block_instruction_node* visitReturn(const std::shared_ptr<peg::Ast>& ast) const;
		block_instruction_node* visitBlock(const std::shared_ptr<peg::Ast>& ast) const;

		type_name_t visitTypeName(const std::shared_ptr<peg::Ast>& ast) const;
		type_name_t visitRegularType(const std::shared_ptr<peg::Ast>& ast) const;
		type_name_t visitFunctionType(const std::shared_ptr<peg::Ast>& ast) const;

		struct_field_t visitField(const std::shared_ptr<peg::Ast>& ast) const;
		struct_field_t visitStandardField(const std::shared_ptr<peg::Ast>& ast) const;
		struct_field_t visitDecoratedField(const std::shared_ptr<peg::Ast>& ast) const;

		std::vector<std::string> m_namespaces;

		const iterator* m_iteratorType;

		std::optional<expression_t> m_parent;
		std::optional<expression_t> m_alpha;
		std::optional<expression_t> m_beta;
	};
}
