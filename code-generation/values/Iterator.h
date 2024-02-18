#ifndef ITERATOR_H
#define ITERATOR_H

#include "BaseValue.h"
#include "EntryPoint.h"
#include "DzResult.h"

#include "metadata/IteratorMetadata.h"

#include "containers/sequence.h"

struct DefaultVisitorContext;

class IIteratable;
class Emitter;
class fragment;

class Iterator : public BaseValueWithMetadata<IteratorMetadata>
{
	using strategy_t = std::vector<DzResult> (Iterator::*)(const Emitter &, DefaultVisitorContext) const;

	public:
		Iterator(const fragment *node, const Type *type, const EntryPoint &entryPoint, const Stack &values, const std::vector<DzResult> &results);

                value_id id() const override;

		const Type *type() const;
                const BaseValue *clone(const EntryPoint &entryPoint, clone_strategy strategy) const;

		std::vector<DzResult> generate(const Emitter &emitter, DefaultVisitorContext context) const;

	private:
		std::vector<DzResult> useRendered(const Emitter &emitter, DefaultVisitorContext context) const;
		std::vector<DzResult> createNew(const Emitter &emitter, DefaultVisitorContext context) const;

		const fragment *m_node;
		const Type *m_type;
		const EntryPoint *m_entryPoint;

		const Stack m_values;

		const std::vector<DzResult> m_results;

		mutable sequence<strategy_t> m_strategies;
};

#endif // ITERATOR_H
