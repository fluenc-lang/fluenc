#include "Node.h"

static size_t s_id;

Node::Node()
	: m_id(s_id++)
{
}

size_t Node::id() const
{
	return m_id;
}

int Node::order(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return -1;
}
