#ifndef GLOBALNODE_H
#define GLOBALNODE_H

#include "Node.h"

class GlobalNode : public Node
{
	friend class Emitter;

	public:
		GlobalNode(Node *value, const std::string &name);

		std::string name() const;

		std::vector<DzResult<BaseValue>> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;

	private:
		Node *m_value;

		std::string m_name;
};

#endif // GLOBALNODE_H
