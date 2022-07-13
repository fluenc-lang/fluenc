#ifndef RETURNNODE_H
#define RETURNNODE_H

#include "Node.h"

class Type;

class ReturnNode : public Node
{
	friend class Emitter;

	public:
		ReturnNode(const Type *iteratorType
			, const Node *consumer
			, const Node *chained
			);

		std::vector<DzResult<BaseValue>> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;

	private:
		const Type *m_iteratorType;

		const Node *m_consumer;
		const Node *m_chained;
};

#endif // RETURNNODE_H
