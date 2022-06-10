#ifndef EXPANDEDVALUE_H
#define EXPANDEDVALUE_H

#include "BaseValue.h"

#include "metadata/ExpandedValueMetadata.h"

class IteratorType;
class Node;

class ExpandedValue : public BaseValueWithMetadata<ExpandedValueMetadata>
{
	public:
		ExpandedValue(const Type *iteratorType, const EntryPoint &provider, const Node *node, const Node *chain, const std::vector<const ExpandedValue *> &next);

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint) const override;

		const EntryPoint *provider() const;
		const Node *node() const;
		const Node *chain() const;

		std::vector<const ExpandedValue *> next() const;

	private:
		const Type *m_iteratorType;
		const EntryPoint *m_provider;
		const Node *m_node;
		const Node *m_chain;

		std::vector<const ExpandedValue *> m_next;
};

#endif // EXPANDEDVALUE_H
