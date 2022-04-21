#ifndef JUNCTIONNODE_H
#define JUNCTIONNODE_H

#include "Node.h"

class JunctionNode : public Node
{
	struct SingleResult
	{
		const EntryPoint entryPoint;
		const BaseValue *value;
	};

	public:
		JunctionNode(const Node *subject);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const BaseValue *join(const std::vector<SingleResult> &range, const EntryPoint &entryPoint) const;

		SingleResult tryJoin(const std::vector<SingleResult> &values, const EntryPoint &entryPoint) const;

		const Node *m_subject;
};

#endif // JUNCTIONNODE_H
