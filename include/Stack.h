#ifndef STACK_H
#define STACK_H

#include <llvm/IR/Value.h>

#include <stack>
#include <vector>

#include "immer/vector.hpp"

#include "exceptions/InvalidTypeException.h"

#include "values/BaseValue.h"

class Stack
{
	public:
		using const_iterator = typename immer::detail::rbts::rbtree_iterator<const BaseValue *, immer::default_memory_policy, immer::default_bits, immer::detail::rbts::derive_bits_leaf<const BaseValue *, immer::default_memory_policy, immer::default_bits>>;
		using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;

		Stack() = default;

		Stack(const std::vector<const BaseValue *> &values);
		Stack(const BaseValue *value);
		Stack(const Stack &other) = default;

		size_t size() const;

		const_iterator begin() const;
		const_iterator end() const;

		const_reverse_iterator rbegin() const;
		const_reverse_iterator rend() const;

		const BaseValue *pop();
		const BaseValue *peek();

		Stack &discard();

		template<typename TValue>
		const TValue *require(const std::shared_ptr<peg::Ast> &ast)
		{
			auto value = pop();

			if (auto casted = dynamic_cast<const TValue *>(value))
			{
				return casted;
			}

			auto &expectedMetadata = TValue::staticMetadata();
			auto &actualMetadata = value->metadata();

			throw new InvalidTypeException(ast
				, expectedMetadata.name()
				, actualMetadata.name()
				);
		}

		template<typename TValue>
		const TValue *request()
		{
			if (size() <= 0)
			{
				return nullptr;
			}

			auto value = pop();

			if (auto casted = dynamic_cast<const TValue *>(value))
			{
				return casted;
			}

			return nullptr;
		}

		void push(const BaseValue *value);

	private:
		immer::vector<const BaseValue *> m_values;
};

#endif // STACK_H
