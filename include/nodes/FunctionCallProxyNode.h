#ifndef FUNCTIONCALLPROXYNODE_H
#define FUNCTIONCALLPROXYNODE_H

#include "Node.h"

class FunctionCallProxyNode : public Node
{
	public:
		FunctionCallProxyNode(const std::vector<std::string> &names
			, const Node *consumer
			, const Node *withEvaluation
			, const Node *withoutEvaluation
			);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::vector<DzResult> regularCall(const EntryPoint &entryPoint, Stack values) const;

		std::vector<std::string> m_names;

		const Node *m_consumer;
		const Node *m_withEvaluation;
		const Node *m_withoutEvaluation;
};

#endif // FUNCTIONCALLPROXYNODE_H
