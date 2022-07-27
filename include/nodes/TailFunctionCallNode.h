#ifndef TAILFUNCTIONCALLNODE_H
#define TAILFUNCTIONCALLNODE_H

#include <numeric>

#include "Node.h"

class TailFunctionCallNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		TailFunctionCallNode(const std::vector<std::string> &names, const Node *regularCall);

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const override;

		int order(const EntryPoint &entryPoint) const override;

	private:
		std::vector<std::string> m_names;

		const Node *m_regularCall;
};

#endif // TAILFUNCTIONCALLNODE_H
