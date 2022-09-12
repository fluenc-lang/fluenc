#ifndef GLOBALNODE_H
#define GLOBALNODE_H

#include "Node.h"

class GlobalNode : public VisitableNode<GlobalNode>
{
	friend class Emitter;
	friend class Analyzer;
	friend class NodeLocator;

	public:
		GlobalNode(Node *value, const std::string &name);

		std::string name() const;

	private:
		Node *m_value;

		std::string m_name;
};

#endif // GLOBALNODE_H
