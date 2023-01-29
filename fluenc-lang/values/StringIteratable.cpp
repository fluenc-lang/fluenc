#include "values/StringIteratable.h"
#include "nodes/IteratableNode.h"

StringIteratable::StringIteratable(const ReferenceValue *index, llvm::Value *address, size_t length)
	: m_index(index)
	, m_node(new IteratableNode(this))
	, m_address(address)
	, m_length(length)
{
}

std::vector<DzResult > StringIteratable::accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const
{
	return visitor.visit(this, context);
}
