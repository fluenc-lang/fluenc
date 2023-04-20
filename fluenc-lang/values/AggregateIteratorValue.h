#ifndef AGGREGATEITERATORVALUE_H
#define AGGREGATEITERATORVALUE_H

#include "IIteratable.h"

class AggregateIteratorValue : public IIteratable
{
	public:
		AggregateIteratorValue(const ReferenceValue *index, const ArrayBinaryNode *node, const Type *arrayType, const std::vector<const BaseValue *> &iteratables);

		std::vector<DzResult> accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const override;

	private:
		const ReferenceValue *m_index;
		const ArrayBinaryNode *m_node;
		const Type *m_arrayType;

		std::vector<const BaseValue *> m_values;
};

#endif // AGGREGATEITERATORVALUE_H
