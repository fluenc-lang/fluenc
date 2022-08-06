#ifndef TYPEDOPERATORNODE_H
#define TYPEDOPERATORNODE_H

#include "NodeVisitor.h"
#include "Node.h"

template<typename TNode>
using VisitorFunction = std::vector<DzResult> (NodeVisitor::*)(const TNode *node, const EntryPoint &entryPoint, Stack values) const;

template<typename TNode, VisitorFunction<TNode> TFunction>
class TypedOperatorNode : public Node
{
	public:
		TypedOperatorNode(const TNode *node)
			: m_node(node)
		{
		}

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
		{
			return (visitor.*TFunction)(m_node, entryPoint, values);
		}

		std::vector<DzResult> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const
		{
			return (visitor.*TFunction)(m_node, entryPoint, values);
		}

	private:
		const TNode *m_node;
};


#endif // TYPEDOPERATORNODE_H
