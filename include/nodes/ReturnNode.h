#ifndef RETURNNODE_H
#define RETURNNODE_H

#include "Node.h"

class Type;

class ReturnNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		ReturnNode(const Type *iteratorType
			, const Node *consumer
			, const Node *chained
			);

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, DefaultVisitorContext context) const override;

	private:
		const Type *m_iteratorType;

		const Node *m_consumer;
		const Node *m_chained;
};

#endif // RETURNNODE_H
