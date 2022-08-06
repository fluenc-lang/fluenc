#include "nodes/CharacterLiteralNode.h"

CharacterLiteralNode::CharacterLiteralNode(const Node *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}

std::vector<DzResult> CharacterLiteralNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitCharacterLiteral(this, entryPoint, values);
}

std::vector<DzResult> CharacterLiteralNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitCharacterLiteral(this, entryPoint, values);
}
