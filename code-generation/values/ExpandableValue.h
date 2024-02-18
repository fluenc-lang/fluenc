#pragma once

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>

#include "BaseValue.h"
#include "Stack.h"

#include "metadata/ExpandableValueMetadata.h"

class Type;
class EntryPoint;
class Node;
class iterator_type;
class expanded_type;

struct fragment;

class ExpandableValue : public BaseValueWithMetadata<ExpandableValueMetadata>
{
	public:
		ExpandableValue(bool isArray
			, const Type *iteratorType
			, const EntryPoint &provider
			, const fragment* chain
			, const Stack &values
			);

                value_id id() const override;

		bool isArray() const;

		const Type *type() const override;
                const BaseValue *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;

		const EntryPoint *provider() const;
		const fragment* chain() const;
                const expanded_type *expandedType() const;
		const Stack values() const;

	private:
		bool m_isArray;

		const Type *m_iteratorType;
		const EntryPoint *m_provider;
		const fragment *m_chain;
		const Stack m_values;
};
