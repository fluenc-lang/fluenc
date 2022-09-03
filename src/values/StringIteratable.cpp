#include "values/StringIteratable.h"

StringIteratable::StringIteratable(const ReferenceValue *index, const Node *node, llvm::Value *address, size_t length)
	: m_index(index)
	, m_node(node)
	, m_address(address)
	, m_length(length)
{
}

std::vector<DzResult > StringIteratable::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitStringIteratable(this, context);
}

std::vector<DzResult > StringIteratable::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitStringIteratable(this, context);
}
