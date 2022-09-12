#include "NodeLocator.h"

#include "nodes/BinaryNode.h"
#include "nodes/ExportedFunctionNode.h"
#include "nodes/ArrayContinuationNode.h"
#include "nodes/ArrayElementNode.h"
#include "nodes/IntegralLiteralNode.h"
#include "nodes/FloatLiteralNode.h"
#include "nodes/BooleanLiteralNode.h"
#include "nodes/StringLiteralNode.h"
#include "nodes/CharacterLiteralNode.h"
#include "nodes/NothingNode.h"
#include "nodes/MemberAccessNode.h"
#include "nodes/ReferenceSinkNode.h"
#include "nodes/LazyEvaluationNode.h"
#include "nodes/FunctionCallNode.h"
#include "nodes/StackSegmentNode.h"
#include "nodes/FunctionCallProxyNode.h"
#include "nodes/JunctionNode.h"
#include "nodes/InstantiationNode.h"
#include "nodes/ConditionalNode.h"
#include "nodes/BlockInstructionNode.h"
#include "nodes/EmptyArrayNode.h"
#include "nodes/IndexSinkNode.h"
#include "nodes/ArraySinkNode.h"
#include "nodes/ExpansionNode.h"
#include "nodes/LocalNode.h"
#include "nodes/ContinuationNode.h"
#include "nodes/UnaryNode.h"
#include "nodes/TailFunctionCallNode.h"
#include "nodes/FunctionNode.h"
#include "nodes/ExportedFunctionTerminatorNode.h"
#include "nodes/ImportedFunctionNode.h"
#include "nodes/GlobalNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/ParentInjectorNode.h"
#include "nodes/BlockStackFrameNode.h"
#include "nodes/TerminatorNode.h"
#include "nodes/IteratableNode.h"

NodeLocator::NodeLocator(Predicate predicate)
	: m_predicate(predicate)
{
}

const Node *NodeLocator::visit(const BooleanBinaryNode *node, DummyVisitorContext context) const
{
	UNUSED(node);
	UNUSED(context);

	return nullptr;
}

const Node *NodeLocator::visit(const FloatBinaryNode *node, DummyVisitorContext context) const
{
	UNUSED(node);
	UNUSED(context);

	return nullptr;
}

const Node *NodeLocator::visit(const IntegerBinaryNode *node, DummyVisitorContext context) const
{
	UNUSED(node);
	UNUSED(context);

	return nullptr;
}

const Node *NodeLocator::visit(const BinaryNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const ExportedFunctionNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_block->accept(*this, context);
}

const Node *NodeLocator::visit(const ArrayContinuationNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_node->accept(*this, context);
}

const Node *NodeLocator::visit(const ArrayElementNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	if (auto result = node->m_node->accept(*this, context))
	{
		return result;
	}

	return node->m_next->accept(*this, context);
}

const Node *NodeLocator::visit(const IntegralLiteralNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const FloatLiteralNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const BooleanLiteralNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const StringLiteralNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const CharacterLiteralNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const NothingNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const MemberAccessNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const ReferenceSinkNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const LazyEvaluationNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const FunctionCallNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_evaluation->accept(*this, context);
}

const Node *NodeLocator::visit(const StackSegmentNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	for (auto value : node->m_values)
	{
		if (auto result = value->accept(*this, context))
		{
			return result;
		}
	}

	if (auto result = node->m_call->accept(*this, context))
	{
		return result;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const FunctionCallProxyNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	if (auto result = node->m_regularCall->accept(*this, context))
	{
		return result;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const JunctionNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_subject->accept(*this, context);
}

const Node *NodeLocator::visit(const InstantiationNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const ConditionalNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	if (auto result = node->m_ifTrue->accept(*this, context))
	{
		return result;
	}

	return node->m_ifFalse->accept(*this, context);
}

const Node *NodeLocator::visit(const BlockInstructionNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_subject->accept(*this, context);
}

const Node *NodeLocator::visit(const EmptyArrayNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const IndexSinkNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const ArraySinkNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	if (auto result = node->m_firstValue->accept(*this, context))
	{
		return result;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const ExpansionNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const LocalNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const ContinuationNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_node->accept(*this, context);
}

const Node *NodeLocator::visit(const UnaryNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const TailFunctionCallNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_regularCall->accept(*this, context);
}

const Node *NodeLocator::visit(const FunctionNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_block->accept(*this, context);
}

const Node *NodeLocator::visit(const ExportedFunctionTerminatorNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return nullptr;
}

const Node *NodeLocator::visit(const ImportedFunctionNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return nullptr;
}

const Node *NodeLocator::visit(const GlobalNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_value->accept(*this, context);
}

const Node *NodeLocator::visit(const ReturnNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	if (node->m_chained)
	{
		if (auto result = node->m_chained->accept(*this, context))
		{
			return result;
		}
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const ParentInjectorNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_subject->accept(*this, context);
}

const Node *NodeLocator::visit(const BlockStackFrameNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const TerminatorNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return nullptr;
}

const Node *NodeLocator::visit(const IteratableNode *node, DummyVisitorContext context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return nullptr;
}

const Node *NodeLocator::visit(const ArrayValue *node, DummyVisitorContext context) const
{
	return nullptr;
}

const Node *NodeLocator::visit(const IteratorValue *node, DummyVisitorContext context) const
{
	return nullptr;
}

const Node *NodeLocator::visit(const StringIteratable *node, DummyVisitorContext context) const
{
	return nullptr;
}
