#ifndef LOCALNODE_H
#define LOCALNODE_H

#include "Node.h"

class LocalNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		LocalNode(const Node *consumer, const std::string &name);

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, DefaultVisitorContext context) const override;

	private:
		const Node *m_consumer;

		std::string m_name;
};


#endif // LOCALNODE_H
