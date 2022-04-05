#ifndef MEMBERACCESS_H
#define MEMBERACCESS_H

#include "Node.h"

class MemberAccess : public Node
{
	public:
		MemberAccess(antlr4::ParserRuleContext *context, Node *consumer, const std::string &name);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		antlr4::ParserRuleContext *m_context;

		Node * m_consumer;

		std::string m_name;
};

#endif // MEMBERACCESS_H
