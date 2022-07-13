#ifndef EXPANSIONNODE_H
#define EXPANSIONNODE_H

#include "Node.h"
#include "TokenInfo.h"

class ExpansionNode : public Node
{
	friend class Emitter;

	public:
		ExpansionNode(const Node *consumer, const std::shared_ptr<peg::Ast> &ast);

		std::vector<DzResult<BaseValue>> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;

	private:
		const Node *m_consumer;

		std::shared_ptr<peg::Ast> m_ast;
};

#endif // EXPANSIONNODE_H
