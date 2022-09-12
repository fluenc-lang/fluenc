#ifndef TAILFUNCTIONCALLNODE_H
#define TAILFUNCTIONCALLNODE_H

#include <numeric>

#include "Node.h"

class TailFunctionCallNode : public VisitableNode<TailFunctionCallNode>
{
	friend class Emitter;
	friend class Analyzer;
	friend class NodeLocator;

	public:
		TailFunctionCallNode(const std::vector<std::string> &names, const Node *regularCall);

		int order(const EntryPoint &entryPoint) const override;

	private:
		std::vector<std::string> m_names;

		const Node *m_regularCall;
};

#endif // TAILFUNCTIONCALLNODE_H
