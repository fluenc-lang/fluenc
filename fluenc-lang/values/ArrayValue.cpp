#include "IndexIterator.h"

#include "values/ArrayValue.h"

#include "nodes/ArrayElementNode.h"
#include "nodes/IteratableNode.h"

ArrayValue::ArrayValue(const std::shared_ptr<peg::Ast> &ast
	, const ReferenceValue *index
	, const Type *type
	, const std::vector<DzResult > &values
	, size_t size
	)
	: m_index(index)
	, m_iterator(createIterator(this, type, size, ast))
	, m_values(values)
{
}

std::vector<DzResult> ArrayValue::accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const
{
	return visitor.visit(this, context);
}

const Node *ArrayValue::createIterator(const IIteratable *parent, const Type *type, size_t size, const std::shared_ptr<peg::Ast> &ast)
{
	return std::accumulate(index_iterator(0u), index_iterator(size), (Node *)nullptr, [&](auto next, auto)
	{
		return new ArrayElementNode(ast, type, new IteratableNode(parent), next);
	});
}
