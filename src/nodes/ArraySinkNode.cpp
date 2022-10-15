#include "nodes/ArraySinkNode.h"

ArraySinkNode::ArraySinkNode(size_t size
	, const std::shared_ptr<peg::Ast> &ast
	, const Node *consumer
	, const Node *firstValue
	)
	: m_size(size)
	, m_ast(ast)
	, m_consumer(consumer)
	, m_firstValue(firstValue)
{
}
