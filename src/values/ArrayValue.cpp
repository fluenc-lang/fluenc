#include "IndexIterator.h"

#include "values/ArrayValue.h"

#include "nodes/ArrayElementNode.h"
#include "nodes/IteratableNode.h"

ArrayValue::ArrayValue(const ReferenceValue *index
	, const Type *type
	, const std::vector<DzResult > &values
	, size_t size
	)
	: m_index(index)
	, m_iterator(createIterator(this, type, size))
	, m_values(values)
{
}

std::vector<DzResult> ArrayValue::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visit(this, context);
}

std::vector<DzResult > ArrayValue::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visit(this, context);
}

const Node *ArrayValue::createIterator(const IIteratable *parent, const Type *type, size_t size)
{
	return std::accumulate(index_iterator(0u), index_iterator(size), (Node *)nullptr, [&](auto next, auto)
	{
		return new ArrayElementNode(type, new IteratableNode(parent), next);
	});
}
