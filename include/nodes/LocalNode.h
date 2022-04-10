#ifndef LOCALNODE_H
#define LOCALNODE_H

#include "Node.h"

class LocalNode : public Node
{
	public:
		LocalNode(const Node *consumer, const std::string &name);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Node *m_consumer;

		std::string m_name;
};


#endif // LOCALNODE_H
