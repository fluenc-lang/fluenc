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

#include "values/FunctionValue.h"

NodeLocator::NodeLocator(Predicate predicate)
	: m_predicate(predicate)
{
}

const Node *NodeLocator::visit(const BooleanBinaryNode *node, EntryPoint context) const
{
	UNUSED(node);
	UNUSED(context);

	return nullptr;
}

const Node *NodeLocator::visit(const FloatBinaryNode *node, EntryPoint context) const
{
	UNUSED(node);
	UNUSED(context);

	return nullptr;
}

const Node *NodeLocator::visit(const IntegerBinaryNode *node, EntryPoint context) const
{
	UNUSED(node);
	UNUSED(context);

	return nullptr;
}

const Node *NodeLocator::visit(const BinaryNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const ExportedFunctionNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_block->accept(*this, context);
}

const Node *NodeLocator::visit(const ArrayContinuationNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_node->accept(*this, context);
}

const Node *NodeLocator::visit(const ArrayElementNode *node, EntryPoint context) const
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

const Node *NodeLocator::visit(const IntegralLiteralNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const FloatLiteralNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const BooleanLiteralNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const StringLiteralNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const CharacterLiteralNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const NothingNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const MemberAccessNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const ReferenceSinkNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const LazyEvaluationNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const FunctionCallNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	auto &functions = context.functions();
	auto &locals = context.locals();

	auto findFunctions = [&](auto name) -> std::vector<const CallableNode *>
	{
		auto local = locals.find(name);

		if (local != locals.end())
		{
			auto value = dynamic_cast<const FunctionValue *>(local->second);

			return { value->function() };
		}

		auto [i, end] = functions.equal_range(name);

		std::vector<const CallableNode *> functions;

		std::transform(i, end, back_inserter(functions), [](auto pair)
		{
			return pair.second;
		});

		return functions;
	};

	for (auto &name : node->m_names)
	{
		for (auto function : findFunctions(name))
		{
			if (auto result = function->accept(*this, context))
			{
				return result;
			}
		}
	}

	return nullptr;
}

const Node *NodeLocator::visit(const StackSegmentNode *node, EntryPoint context) const
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

const Node *NodeLocator::visit(const FunctionCallProxyNode *node, EntryPoint context) const
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

const Node *NodeLocator::visit(const JunctionNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_subject->accept(*this, context);
}

const Node *NodeLocator::visit(const InstantiationNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const ConditionalNode *node, EntryPoint context) const
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

const Node *NodeLocator::visit(const BlockInstructionNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_subject->accept(*this, context);
}

const Node *NodeLocator::visit(const EmptyArrayNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const IndexSinkNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const ArraySinkNode *node, EntryPoint context) const
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

const Node *NodeLocator::visit(const ExpansionNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const LocalNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const ContinuationNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_node->accept(*this, context);
}

const Node *NodeLocator::visit(const UnaryNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const TailFunctionCallNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_regularCall->accept(*this, context);
}

const Node *NodeLocator::visit(const FunctionNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_block->accept(*this, context);
}

const Node *NodeLocator::visit(const ExportedFunctionTerminatorNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return nullptr;
}

const Node *NodeLocator::visit(const ImportedFunctionNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return nullptr;
}

const Node *NodeLocator::visit(const GlobalNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_value->accept(*this, context);
}

const Node *NodeLocator::visit(const ReturnNode *node, EntryPoint context) const
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

const Node *NodeLocator::visit(const ParentInjectorNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_subject->accept(*this, context);
}

const Node *NodeLocator::visit(const BlockStackFrameNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return node->m_consumer->accept(*this, context);
}

const Node *NodeLocator::visit(const TerminatorNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return nullptr;
}

const Node *NodeLocator::visit(const IteratableNode *node, EntryPoint context) const
{
	if (m_predicate(node))
	{
		return node;
	}

	return nullptr;
}

const Node *NodeLocator::visit(const ArrayValue *node, EntryPoint context) const
{
	return nullptr;
}

const Node *NodeLocator::visit(const IteratorValue *node, EntryPoint context) const
{
	return nullptr;
}

const Node *NodeLocator::visit(const StringIteratable *node, EntryPoint context) const
{
	return nullptr;
}
