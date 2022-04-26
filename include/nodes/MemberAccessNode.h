#ifndef MEMBERACCESSNODE_H
#define MEMBERACCESSNODE_H

#include "Node.h"
#include "TokenInfo.h"

class MemberAccessNode : public Node
{
	public:
		MemberAccessNode(const Node *consumer
			, const std::shared_ptr<peg::Ast> &ast
			, const std::vector<std::string> &names
			);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Node * m_consumer;

		std::shared_ptr<peg::Ast> m_ast;
		std::vector<std::string> m_names;
};

#endif // MEMBERACCESSNODE_H
