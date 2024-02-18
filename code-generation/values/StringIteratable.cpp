#include "values/StringIteratable.h"

#include "fragments/IteratableNode.h"

#include "Emitter.h"

StringIteratable::StringIteratable(const reference_value *index, const reference_value *address, const reference_value *length)
	: m_index(index)
	, m_node(new IteratableNode(this))
	, m_address(address)
	, m_length(length)
{
}

std::vector<DzResult > StringIteratable::build(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visit(this, context);
}
