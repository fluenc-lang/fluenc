#ifndef JUNCTIONNODE_H
#define JUNCTIONNODE_H

#include "Node.h"

class JunctionNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		JunctionNode(const Node *subject);

		std::vector<DzResult<BaseValue>> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;

	private:
		const Node *m_subject;
};

#endif // JUNCTIONNODE_H
