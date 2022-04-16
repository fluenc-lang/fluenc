#ifndef EXPANSIONNODE_H
#define EXPANSIONNODE_H

#include "Node.h"
#include "TokenInfo.h"

class ExpansionNode : public Node
{
	public:
		ExpansionNode(antlr4::ParserRuleContext *context, const Node *consumer);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		TokenInfo m_token;

		const Node *m_consumer;
};

#endif // EXPANSIONNODE_H
