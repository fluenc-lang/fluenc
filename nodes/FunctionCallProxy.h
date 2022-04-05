#ifndef FUNCTIONCALLPROXY_H
#define FUNCTIONCALLPROXY_H

#include "Node.h"

class FunctionCallProxy : public Node
{
	public:
		FunctionCallProxy(const std::string name, const Node *consumer, const Node *candidate);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::vector<DzResult> regularCall(const EntryPoint &entryPoint, Stack values) const;

		std::string m_name;

		const Node *m_consumer;
		const Node *m_subject;
};

#endif // FUNCTIONCALLPROXY_H
