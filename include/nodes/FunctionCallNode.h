#ifndef FUNCTIONCALLNODE_H
#define FUNCTIONCALLNODE_H

#include "Node.h"
#include "TokenInfo.h"

class FunctionCallNode : public Node
{
	public:
		FunctionCallNode(const std::shared_ptr<peg::Ast> &ast, const std::vector<std::string> &names, const Node *evaluation);

		int order(const EntryPoint &entryPoint) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const CallableNode *findFunction(const EntryPoint &entryPoint, Stack values) const;

		std::shared_ptr<peg::Ast> m_ast;
		std::vector<std::string> m_names;

		const Node *m_evaluation;
};

#endif // FUNCTIONCALLNODE_H
