#include "values/StringIteratable.h"

StringIteratable::StringIteratable(const ReferenceValue *index, const Node *node, llvm::Value *address, size_t length)
	: m_index(index)
	, m_node(node)
	, m_address(address)
	, m_length(length)
{
}

std::vector<DzResult<BaseValue> > StringIteratable::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitStringIteratable(this, entryPoint, values);
}

std::vector<DzResult<BaseValue> > StringIteratable::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitStringIteratable(this, entryPoint, values);
}
