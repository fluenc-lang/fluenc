#ifndef ITERATOR_H
#define ITERATOR_H

#include "BaseValue.h"
#include "EntryPoint.h"
#include "DzResult.h"
#include "Node.h"

#include "metadata/IteratorMetadata.h"

#include "containers/sequence.h"

struct DefaultVisitorContext;

class Iterator : public BaseValueWithMetadata<IteratorMetadata>
{
	using strategy_t = std::vector<DzResult> (Iterator::*)(const DefaultNodeVisitor &, DefaultVisitorContext) const;

	public:
		Iterator(const Node *node, const Type *type, const EntryPoint &entryPoint, const Stack &values, const std::vector<DzResult> &results);

		ValueId id() const override;

		const Type *type() const;
		const BaseValue *clone(const EntryPoint &entryPoint, CloneStrategy strategy) const;

		std::vector<DzResult> generate(const DefaultNodeVisitor &emitter, DefaultVisitorContext context) const;

	private:
		std::vector<DzResult> useRendered(const DefaultNodeVisitor &emitter, DefaultVisitorContext context) const;
		std::vector<DzResult> createNew(const DefaultNodeVisitor &emitter, DefaultVisitorContext context) const;

		const Node *m_node;
		const Type *m_type;
		const EntryPoint *m_entryPoint;

		const Stack m_values;

		const std::vector<DzResult> m_results;

		mutable sequence<strategy_t> m_strategies;
};

#endif // ITERATOR_H
