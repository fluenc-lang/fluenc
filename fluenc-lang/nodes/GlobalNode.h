#ifndef GLOBALNODE_H
#define GLOBALNODE_H

#include "Node.h"

class GlobalNode : public VisitableNode<GlobalNode>
{
	friend class Emitter;

	public:
		GlobalNode(const std::shared_ptr<peg::Ast> &ast, Node *value, const std::string &name);

		const std::shared_ptr<peg::Ast> ast() const override;

		std::string name() const;

	private:
		const std::shared_ptr<peg::Ast> m_ast;

		Node *m_value;

		std::string m_name;
};

#endif // GLOBALNODE_H
