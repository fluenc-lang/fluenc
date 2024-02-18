#include "ArrayValue.h"
#include "Emitter.h"

#include "iterators/index_iterator.hpp"

#include "fragments/ArrayElementNode.h"
#include "fragments/IteratableNode.h"

ArrayValue::ArrayValue(const std::shared_ptr<peg::Ast> &ast
                       , const reference_value *index
	, const Type *type
	, const std::vector<DzResult > &values
	, size_t size
	)
	: m_index(index)
	, m_iterator(createIterator(this, type, size, ast))
	, m_values(values)
{
}

std::vector<DzResult> ArrayValue::build(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visit(this, context);
}

fragment* ArrayValue::createIterator(const fragment *parent, const Type *type, size_t size, const std::shared_ptr<peg::Ast> &ast)
{
	return std::accumulate(fluenc::index_iterator(0u), fluenc::index_iterator(size), static_cast<fragment*>(nullptr), [&](auto next, auto) -> fragment*
	{
		return new ArrayElementNode(ast, type, new IteratableNode(parent), next);
	});
}
