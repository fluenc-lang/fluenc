#ifndef MEMBERACCESSNODE_H
#define MEMBERACCESSNODE_H

#include "Node.h"
#include "TokenInfo.h"

class MemberAccessNode : public Node
{
	public:
		MemberAccessNode(antlr4::ParserRuleContext *context, const Node *consumer, const std::vector<std::string> &names);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		TokenInfo m_token;

		const Node * m_consumer;

		std::vector<std::string> m_names;
};

#endif // MEMBERACCESSNODE_H
