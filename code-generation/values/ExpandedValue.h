#pragma once

#include "BaseValue.h"
#include "Stack.h"

#include "metadata/ExpandedValueMetadata.h"

class iterator_type;
class Node;
class IIteratable;

class fragment;

class ExpandedValue : public BaseValueWithMetadata<ExpandedValueMetadata>
{
	public:
		ExpandedValue(bool isArray
			, const Type *iteratorType
			, const EntryPoint &provider
			, const fragment *node
			// , const fragment *chain
			, const std::vector<const ExpandedValue *> &next
			, const Stack &values
			);

                value_id id() const override;

		bool isArray() const;

		const Type *type() const override;
                const BaseValue *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;

		const EntryPoint *provider() const;
		const fragment *node() const;
		// const fragment *chain() const;

		std::vector<const ExpandedValue *> next() const;

		const Stack &values() const;

	private:
		bool m_isArray;

		const Type *m_iteratorType;
		const EntryPoint *m_provider;
		const fragment* m_node;
		const fragment* m_chain;

		std::vector<const ExpandedValue *> m_next;

		Stack m_values;
};
