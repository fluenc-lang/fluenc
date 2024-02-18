#pragma once

#include "ast/argument_node.hpp"
#include "ast/array_node.hpp"
#include "ast/binary_node.hpp"
#include "ast/block_instruction_node.hpp"
#include "ast/boolean_literal_node.hpp"
#include "ast/character_literal_node.hpp"
#include "ast/conditional_node.hpp"
#include "ast/expansion_node.hpp"
#include "ast/extension_node.hpp"
#include "ast/float_literal_node.hpp"
#include "ast/function_call_node.hpp"
#include "ast/function_node.hpp"
#include "ast/global_node.hpp"
#include "ast/instantiation_node.hpp"
#include "ast/integral_literal_node.hpp"
#include "ast/local_node.hpp"
#include "ast/member_access_node.hpp"
#include "ast/nothing_node.hpp"
#include "ast/return_node.hpp"
#include "ast/string_literal_node.hpp"
#include "ast/struct_node.hpp"
#include "ast/terminator_node.hpp"
#include "ast/tuple_sink_node.hpp"
#include "ast/unary_node.hpp"

namespace fluenc
{
	template <typename TVisitor, typename... T>
	typename TVisitor::return_type accept(
		const std::variant<T...>& visitable,
		TVisitor& visitor,
		typename TVisitor::context_type context
	)
	{
		return std::visit(
			[&](auto&& node) {
				return visitor.visit(node, context);
			},
			visitable
		);
	}

	template <typename TVisitor>
	typename TVisitor::return_type accept(const auto& node, TVisitor& visitor, typename TVisitor::context_type context)
	{
		return visitor.visit(node, context);
	}

	template <typename T> const T* ast_cast(const expression_t& visitable)
	{
		return std::visit(
			[&](auto* arg) -> const T* {
				if constexpr (std::is_convertible_v<decltype(arg), const T*>)
				{
					return static_cast<const T*>(arg);
				}
				else
				{
					return nullptr;
				}
			},
			visitable
		);
	}

	template <typename T, typename U> const T* ast_cast(const std::optional<U>& visitable)
	{
		if (visitable)
		{
			return ast_cast<T>(*visitable);
		}

		return nullptr;
	}
} // namespace fluenc
