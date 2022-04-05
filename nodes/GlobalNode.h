#ifndef GLOBALNODE_H
#define GLOBALNODE_H

#include "Node.h"

class GlobalNode : public Node
{
	public:
		GlobalNode(Node *value, const std::string &name);

		std::string name() const;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		Node *m_value;

		std::string m_name;
};

#endif // GLOBALNODE_H
