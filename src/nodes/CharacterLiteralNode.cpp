#include "nodes/CharacterLiteralNode.h"

CharacterLiteralNode::CharacterLiteralNode(const Node *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}

std::vector<DzResult> CharacterLiteralNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitCharacterLiteral(this, context);
}

std::vector<DzResult> CharacterLiteralNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitCharacterLiteral(this, context);
}
