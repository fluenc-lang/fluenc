#pragma once

#include "ast/binary_node.hpp"

#include "emitter.hpp"
#include "fragment.hpp"
#include "ir_builder.hpp"
#include "utility.hpp"
#include "value_helper.hpp"

#include "values/pre_lazy_value.hpp"
#include "values/scalar_value.hpp"

#include "exceptions/invalid_operator_exception.hpp"

#include <llvm/IR/Constants.h>

namespace fluenc::code_generation::fragments
{
	struct integer_binary_fragment : public fragment
	{
		integer_binary_fragment(const binary_node* binary)
			: binary_(binary)
		{
		}

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override
		{
			UNUSED(visitor);

			auto left = value_helper::get_scalar(binary_->ast, context.entryPoint, context.values);
			auto right = value_helper::get_scalar(binary_->ast, context.entryPoint, context.values);

			ir_builder builder(context.entryPoint);

			auto valueFactory = [&] {
				if (binary_->op == "+")
				{
					return builder.create_add(left, right);
				}

				if (binary_->op == "-")
				{
					return builder.create_sub(left, right);
				}

				if (binary_->op == "*")
				{
					return builder.create_mul(left, right);
				}

				if (binary_->op == "/")
				{
					return builder.create_sdiv(left, right);
				}

				if (binary_->op == "<")
				{
					return builder.create_cmp(llvm::CmpInst::Predicate::ICMP_SLT, left, right);
				}

				if (binary_->op == "<=")
				{
					return builder.create_cmp(llvm::CmpInst::Predicate::ICMP_SLE, left, right);
				}

				if (binary_->op == ">")
				{
					return builder.create_cmp(llvm::CmpInst::Predicate::ICMP_SGT, left, right);
				}

				if (binary_->op == ">=")
				{
					return builder.create_cmp(llvm::CmpInst::Predicate::ICMP_SGE, left, right);
				}

				if (binary_->op == "==")
				{
					return builder.create_cmp(llvm::CmpInst::Predicate::ICMP_EQ, left, right);
				}

				if (binary_->op == "!=")
				{
					return builder.create_cmp(llvm::CmpInst::Predicate::ICMP_NE, left, right);
				}

				if (binary_->op == "%")
				{
					return builder.create_srem(left, right);
				}

				if (binary_->op == "|")
				{
					return builder.create_or(left, right);
				}

				if (binary_->op == "&")
				{
					return builder.create_and(left, right);
				}

				if (binary_->op == "^")
				{
					return builder.create_xor(left, right);
				}

				if (binary_->op == "<<")
				{
					return builder.create_left_shift(left, right);
				}

				if (binary_->op == ">>")
				{
					return builder.create_right_shift(left, right);
				}

				auto operandType = left->type();
				auto operandTypeName = operandType->name();

				throw invalid_operator_exception(binary_->ast, binary_->op, operandTypeName);
			};

			auto value = valueFactory();

			context.values.push(value);

			return { { context.entryPoint, context.values } };
		}

	private:
		const binary_node* binary_;
	};

	struct boolean_binary_fragment : public fragment
	{
		boolean_binary_fragment(const binary_node* binary)
			: binary_(binary)
		{
		}

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override
		{
			UNUSED(visitor);

			auto left = value_helper::get_scalar(binary_->ast, context.entryPoint, context.values);
			auto right = value_helper::get_scalar(binary_->ast, context.entryPoint, context.values);

			ir_builder builder(context.entryPoint);

			auto valueFactory = [&] {
				if (binary_->op == "==")
				{
					return builder.create_cmp(llvm::CmpInst::Predicate::ICMP_EQ, left, right);
				}

				if (binary_->op == "!=")
				{
					return builder.create_cmp(llvm::CmpInst::Predicate::ICMP_NE, left, right);
				}

				if (binary_->op == "&&")
				{
					return builder.create_logical_and(left, right);
				}

				if (binary_->op == "||")
				{
					return builder.create_logical_or(left, right);
				}

				auto operandType = left->type();
				auto operandTypeName = operandType->name();

				throw invalid_operator_exception(binary_->ast, binary_->op, operandTypeName);
			};

			auto value = valueFactory();

			context.values.push(value);

			return { { context.entryPoint, context.values } };
		}

	private:
		const binary_node* binary_;
	};

	struct float_binary_fragment : public fragment
	{
		float_binary_fragment(const binary_node* binary)
			: binary_(binary)
		{
		}

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override
		{
			UNUSED(visitor);

			auto left = value_helper::get_scalar(binary_->ast, context.entryPoint, context.values);
			auto right = value_helper::get_scalar(binary_->ast, context.entryPoint, context.values);

			ir_builder builder(context.entryPoint);

			auto valueFactory = [&] {
				if (binary_->op == "+")
				{
					return builder.create_fadd(left, right);
				}

				if (binary_->op == "-")
				{
					return builder.create_fsub(left, right);
				}

				if (binary_->op == "*")
				{
					return builder.create_fmul(left, right);
				}

				if (binary_->op == "/")
				{
					return builder.create_fdiv(left, right);
				}

				if (binary_->op == "<")
				{
					return builder.create_cmp(llvm::CmpInst::Predicate::FCMP_ULT, left, right);
				}

				if (binary_->op == "<=")
				{
					return builder.create_cmp(llvm::CmpInst::Predicate::FCMP_ULE, left, right);
				}

				if (binary_->op == ">")
				{
					return builder.create_cmp(llvm::CmpInst::Predicate::FCMP_UGT, left, right);
				}

				if (binary_->op == ">=")
				{
					return builder.create_cmp(llvm::CmpInst::Predicate::FCMP_UGE, left, right);
				}

				if (binary_->op == "==")
				{
					return builder.create_cmp(llvm::CmpInst::Predicate::FCMP_UEQ, left, right);
				}

				if (binary_->op == "!=")
				{
					return builder.create_cmp(llvm::CmpInst::Predicate::FCMP_UNE, left, right);
				}

				auto operandType = left->type();
				auto operandTypeName = operandType->name();

				throw invalid_operator_exception(binary_->ast, binary_->op, operandTypeName);
			};

			auto value = valueFactory();

			context.values.push(value);

			return { { context.entryPoint, context.values } };
		}

	private:
		const binary_node* binary_;
	};

	struct string_binary_fragment : public fragment
	{
		string_binary_fragment(const binary_node* binary)
			: binary_(binary)
		{
		}

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override
		{
			UNUSED(visitor);

			auto operand = context.values.pop();

			auto operandType = operand->type();
			auto operandTypeName = operandType->name();

			throw invalid_operator_exception(binary_->ast, binary_->op, operandTypeName);
		}

	private:
		const binary_node* binary_;
	};

	struct array_binary_fragment : public fragment
	{
		array_binary_fragment(const binary_node* binary)
			: binary_(binary)
		{
		}

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override;

	private:
		const binary_node* binary_;
	};

	struct without_binary_fragment : public fragment
	{
		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override
		{
			UNUSED(visitor);

			context.values.discard();

			return { { context.entryPoint, context.values } };
		}
	};

	struct user_binary_fragment : public fragment
	{
		user_binary_fragment(const binary_node* binary)
			: binary_(binary)
		{
		}

		std::vector<emitter_result> build(const emitter& visitor, emitter_context context) const override
		{
			UNUSED(visitor);

			auto left = context.values.pop();
			auto right = context.values.pop();

			if (binary_->op == "|")
			{
				auto tuple = new values::tuple_value({ right, left });

				context.values.push(tuple);

				return { { context.entryPoint, context.values } };
			}

			auto operandType = left->type();
			auto operandTypeName = operandType->name();

			throw invalid_operator_exception(binary_->ast, binary_->op, operandTypeName);
		}

	private:
		const binary_node* binary_;
	};
}
