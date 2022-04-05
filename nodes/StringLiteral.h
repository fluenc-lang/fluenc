#ifndef STRINGLITERAL_H
#define STRINGLITERAL_H

#include "Node.h"

class StringLiteral : public Node
{
	public:
		StringLiteral(Node *consumer, const std::string &value);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		Node *m_consumer;

		std::string m_value;
};

#endif // STRINGLITERAL_H
