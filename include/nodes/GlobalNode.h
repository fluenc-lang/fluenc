#ifndef GLOBALNODE_H
#define GLOBALNODE_H

#include "Node.h"

class GlobalNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		GlobalNode(Node *value, const std::string &name);

		std::string name() const;

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, DefaultVisitorContext context) const override;

	private:
		Node *m_value;

		std::string m_name;
};

#endif // GLOBALNODE_H
