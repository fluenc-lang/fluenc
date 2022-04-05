#ifndef LOCAL_H
#define LOCAL_H

#include "Node.h"

class Local : public Node
{
	public:
		Local(const Node *consumer, const std::string &name);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Node *m_consumer;

		std::string m_name;
};


#endif // LOCAL_H
