#ifndef MEMBERACCESSNODE_H
#define MEMBERACCESSNODE_H

#include "Node.h"
#include "TokenInfo.h"

class MemberAccessNode : public Node
{
	public:
		MemberAccessNode(antlr4::ParserRuleContext *context, Node *consumer, const std::string &name);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		TokenInfo m_token;

		Node * m_consumer;

		std::string m_name;
};

#endif // MEMBERACCESSNODE_H
