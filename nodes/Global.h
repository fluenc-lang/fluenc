#ifndef GLOBAL_H
#define GLOBAL_H

#include "Node.h"

class Global : public Node
{
	public:
		Global(Node *value, const std::string &name);

		std::string name() const;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		Node *m_value;

		std::string m_name;
};

#endif // GLOBAL_H
