#ifndef TYPEDOPERATORNODE_H
#define TYPEDOPERATORNODE_H

#include "NodeVisitor.h"
#include "Node.h"

template<typename TNode>
using VisitorFunction = std::vector<DzResult> (DefaultNodeVisitor::*)(const TNode *node, DefaultVisitorContext context) const;

template<typename TNode, VisitorFunction<TNode> TFunction>
class TypedOperatorNode : public Node
{
	public:
		TypedOperatorNode(const TNode *node)
			: m_node(node)
		{
		}

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const
		{
			return (visitor.*TFunction)(m_node, context);
		}

		std::vector<DzResult> accept(const Analyzer &visitor, DefaultVisitorContext context) const
		{
			return (visitor.*TFunction)(m_node, context);
		}

	private:
		const TNode *m_node;
};


#endif // TYPEDOPERATORNODE_H
