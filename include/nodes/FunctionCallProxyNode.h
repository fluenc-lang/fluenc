#ifndef FUNCTIONCALLPROXYNODE_H
#define FUNCTIONCALLPROXYNODE_H

#include "Node.h"

class FunctionCallProxyNode : public VisitableNode<FunctionCallProxyNode>
{
	friend class Emitter;
	friend class Analyzer;
	friend class NodeLocator;

	public:
		FunctionCallProxyNode(const std::vector<std::string> &names
			, const Node *regularCall
			, const Node *consumer
			);

	private:
		std::vector<std::string> m_names;

		const Node *m_regularCall;
		const Node *m_consumer;
};

#endif // FUNCTIONCALLPROXYNODE_H
